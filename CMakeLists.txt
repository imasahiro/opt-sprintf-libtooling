set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(qrintf-libtooling
  SprintfConvert.cpp
  IncludeDirectives.cpp
)
target_link_libraries(qrintf-libtooling
  clangTooling
  clangBasic
  clangASTMatchers
)
