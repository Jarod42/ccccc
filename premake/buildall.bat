@set premake_exe=premake5.exe
@set premake_entry=premake5.lua

set LLVM_ROOT=C:/msys64/mingw64

"%premake_exe%" --file="%premake_entry%" codelite
@rem "%premake_exe%" --file="%premake_entry%" codeblocks
"%premake_exe%" --file="%premake_entry%" gmake2
@rem "%premake_exe%" --file="%premake_entry%" --llvm-root="%LLVM_ROOT%" vs2022

@pause
