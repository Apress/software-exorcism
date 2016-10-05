Greetings Reader,

Reverend Bill here. I decided to exclude trivial source code examples 
in favor of the lengthy ones. Most of the source code in this
section was built with the following three tools:

1) DJGPP (i.e. GNU gcc compiler for Intel )
2) MASM  ( Microsoft's x86 assembler ) 
3) Visual C++ v. 6.0 (can probably use .NET C++ compiler)
4) Visual C++ v. 1.52 (whoa, what a clunker!)

There are no special installation Notes. Although I have included
comments below for gotchas.

In some cases, I have included pre-built binaries to prevent 
hassles. 

Watch your back,
Rev. Bill

+++++++++++
+CHAPTER-1+
+++++++++++

BufferOverflow.c	built with Visual Studio v.6.0 ( uses MS inline assembler )
LogFramework.cpp	built with Visual Studio v.6.0 
WindowsTime.c		built with Visual Studio v.6.0 ( need Win32 )
UnitTest.cpp		built with Visual Studio v.6.0

+++++++++++
+CHAPTER-2+
+++++++++++

testBDW.c		built with Visual Studio v.6.0		
dangling.c		built with Visual Studio v.6.0
stackGarbage.c		built with Visual Studio v.6.0
checkStack.c		built with Visual Studio v.6.0
testProfiler.c		built with DJGPP
gc.lib			pre-built binary for BDW (single-threaded, static)
profileOutput.txt	output produced by testProfiler.c


+++++++++++
+CHAPTER-3+
+++++++++++

UseEveryFeature.c	built with Visual Studio v.6.0
CutAndPaste.c		built with Visual Studio v.6.0
SpaghettiCode.c		built with Visual Studio v.6.0
alias.c			built with Visual Studio v.6.0

+++++++++++
+CHAPTER-4+
+++++++++++

simple.c		built with Visual C++ v.1.52
dosdebug.c		built with Visual C++ v.1.52
saltCode.c		built with Visual Studio v.6.0
bytecode.c		built with Visual Studio v.6.0
simple2.c		built with Visual Studio v.6.0
vm.cpp			built with Visual Studio v.6.0
windebug.cpp		built with Visual Studio v.6.0
program.run		bytecode executable run by vm.exe

+++++++++++
+CHAPTER-5+
+++++++++++

cutAndPaste.c		built with Visual Studio v.6.0
single.asm		built with MASM (assembly code)
Object.h		built with Visual Studio v.6.0
allSegments.c		built with Visual Studio v.6.0
zlib.dll		pre-built zlib DLL 
useZlibDLL.c		built with Visual Studio v.6.0 (uses windows DLL)
lazyClass.cpp		built with Visual Studio v.6.0
pool.cpp		built with Visual Studio v.6.0

+++++++++++
+CHAPTER-6+
+++++++++++

extendableArray.cpp	built with Visual Studio v.6.0
synchronize.c		built with Visual Studio v.6.0 (use Win32 API)
varyArgs.c		built with Visual Studio v.6.0
floatingPoint.c		built with Visual Studio v.6.0

"Do you think you're better than other people? Well, you probably are."
-Dobbs 1961
Praise Bob