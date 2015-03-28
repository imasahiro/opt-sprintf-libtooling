# qrintf-libtooling
sprintf accelerator using libtooling

## Install

For convenience, here is a possible way to build the project:

    $ svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm
    $ cd llvm/tools
    $ svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
    $ cd ../..
    $ cd llvm/projects
    $ svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
    $ cd ../..
    $ cd llvm/tools/clang/tools
    $ svn co http://llvm.org/svn/llvm-project/clang-tools-extra/trunk extra
    $ cd ../../../..
    $ mkdir -p build/target
    $ cd llvm/tools/clang/tools/extra
    $ git clone https://github.com/imasahiro/qrintf-libtooling.git
    $ echo 'add_subdirectory(qrintf-libtooling)' >> tools/extra/CMakeLists.txt
    $ cd ../../../../../build
    $ ../llvm/configure --prefix=`pwd`/target
    $ make
    $ make check
    $ make install

## Notice
./IncludeDirectives.{cpp,h} is copied from
```llvm-3.5.1/tools/clang/tools/extra/clang-modernize/Core/````
