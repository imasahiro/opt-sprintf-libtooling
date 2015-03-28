#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

#include "IncludeDirectives.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;
using namespace std;

struct IncludesManager : public tooling::SourceFileCallbacks {
  IncludesManager() : Includes(0) {}
  ~IncludesManager() {
    delete Includes;
  }
  IncludeDirectives* Includes;
  virtual bool handleBeginSource(CompilerInstance &CI, StringRef Filename)
      override;
};

bool IncludesManager::handleBeginSource(CompilerInstance &CI, StringRef Filename)
{
  // llvm::errs() << Filename << "\n";
  Includes = new IncludeDirectives(CI);
  return true;
}

#include "SprintfTransform.h"

/*
 * int sprintf(
 *         char *str,
 *         const char *format,
 *         ...);
 * int snprintf(
 *         char *str,
 *         size_t size,
 *         const char *format,
 *         ...);
 * int __sprintf_chk(
 *         char       *str,
 *         int         flag,
 *         size_t      strlen,
 *         const char *format,
 *         ...);
 * int __snprintf_chk(
 *         char       *str,
 *         size_t      maxlen,
 *         int         flag,
 *         size_t      strlen,
 *         const char *format,
 *         ...);
 */
struct SprintfData {
    string name;
    int str;
    int buflen;
    int format;
};
static struct SprintfData FnSprintfData[] = {
    {"sprintf",        0, -1, 1},
    {"snprintf",       0,  1, 2},
    {"__builtin___sprintf_chk",  0, -1, 3},
    {"__builtin___snprintf_chk", 0,  1, 4}
};

StatementMatcher makeSprintfMatcher(SprintfData *SI) {
  return callExpr(
      callee(functionDecl(hasName(SI->name))),
      hasArgument(SI->format, id("fmt",stringLiteral()))
      ).bind(SI->name);
}

class SprintfReplacer : public MatchFinder::MatchCallback {
private:
  Replacements *Replace;
  IncludesManager *IM;
public :
  SprintfReplacer(Replacements *Replace, IncludesManager *IM) :
    Replace(Replace), IM(IM) {}

  virtual void run(const MatchFinder::MatchResult &Result) override;
};

// original code was copied from
// clang/tools/extra/remove-cstr-calls/RemoveCStrCalls.cpp
static string getText(SourceManager &SM, const Expr *Node, SourceLocation *Loc)
{
  SourceLocation LocStart = SM.getSpellingLoc(Node->getLocStart());
  SourceLocation LocEnd   = SM.getSpellingLoc(Node->getLocEnd());
  if (!LocStart.isValid() || !LocEnd.isValid()) {
    return string();
  }
  bool Invalid = true;
  const char *Text = SM.getCharacterData(LocStart, &Invalid);
  if (Invalid) {
    return string();
  }
  pair<FileID, unsigned> Start = SM.getDecomposedLoc(LocStart);
  pair<FileID, unsigned> End   = SM.getDecomposedLoc(
      Lexer::getLocForEndOfToken(
        LocEnd, 0, SM, LangOptions()));
  if (Start.first != End.first) {
    // Start and end are in different files.
    return string();
  }
  if (End.second < Start.second) {
    // Shuffling text with macros may cause this.
    return string();
  }
  *Loc = LocStart.getLocWithOffset(End.second - Start.second);
  return string(Text, End.second - Start.second);
}

void addIncludeHeader(
  const FileEntry *File,
  Replacements &Replace,
  IncludeDirectives &Includes) {

  const Replacement &R = Includes.addAngledInclude(File, "qrintf.h");
  if (R.isApplicable()) {
    Replace.insert(R);
  }
}

void SprintfReplacer::run(const MatchFinder::MatchResult &Result)
{
  SprintfOptimizer SO(true);
  SprintfData *SI = &FnSprintfData[3];
  const CallExpr *CE = Result.Nodes.getNodeAs<CallExpr>(SI->name);
  const StringLiteral *SL = Result.Nodes.getNodeAs<StringLiteral>("fmt");
  assert((CE || SL) && "Bad Callback. No node provided");

  SourceManager &SM = *Result.SourceManager;
  SourceLocation LocStart = SM.getExpansionLoc(CE->getLocStart());
  if (!SM.isWrittenInMainFile(LocStart)) {
    llvm::errs() << "Ignore file: " << SM.getFilename(LocStart) << '\n';
    return;
  }

  if (SO.check(SL->getBytes())) {
    SourceLocation LocEnd = LocStart;
    vector<string> args;
    string res = "";
    string buffer = getText(SM, CE->getArg(0), &LocEnd);
    string buflen = getText(SM, CE->getArg(SI->buflen), &LocEnd);
    for (unsigned i = SI->format + 1; i < CE->getNumArgs(); i++) {
      const Expr *E = CE->getArg(i);
      args.push_back(getText(SM, E, &LocEnd));
    }
    if (SO.apply(SL->getBytes(), buffer, buflen, args, &res)) {
      const FileEntry *File = SM.getFileEntryForID(SM.getFileID(LocStart));
      addIncludeHeader(File, *Replace, *IM->Includes);

      // cout << res << "\n";
      CharSourceRange CSR = CharSourceRange::getTokenRange(LocStart, LocEnd);
      Replace->insert(Replacement(SM, CSR, res));
    }
  }
  else {
  }
}

// Set up the command line options
cl::opt<string> BuildPath(
  cl::Positional,
  cl::desc("<build-path>"));

cl::list<string> SourcePaths(
  cl::Positional,
  cl::desc("<source0> [... <sourceN>]"),
  cl::OneOrMore);

int main(int argc, const char **argv) {
  unique_ptr<CompilationDatabase> Compilations(
      FixedCompilationDatabase::loadFromCommandLine(argc, argv));
  cl::ParseCommandLineOptions(argc, argv);
  if (!Compilations) {  // Couldn't find a compilation DB from the command line
    string ErrorMessage;
    Compilations.reset(
      !BuildPath.empty() ?
        CompilationDatabase::autoDetectFromDirectory(BuildPath, ErrorMessage) :
        CompilationDatabase::autoDetectFromSource(SourcePaths[0], ErrorMessage)
      );
    //  Still no compilation DB? - bail.
    if (!Compilations)
      llvm::report_fatal_error(ErrorMessage);
    }

  IncludesManager IM;
  RefactoringTool Tool(*Compilations, SourcePaths);

  Replacements *Replace = &Tool.getReplacements();
  SprintfReplacer Sprintfs(Replace, &IM);
  MatchFinder Finder;
  Finder.addMatcher(makeSprintfMatcher(&FnSprintfData[3]), &Sprintfs);
  return Tool.runAndSave(newFrontendActionFactory(&Finder, &IM).get());
}
