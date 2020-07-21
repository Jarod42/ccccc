@set premake_exe=Premake4.exe
@set premake_entry=premake4.lua

set LLVM_ROOT=C:/Program Files/LLVM

"%premake_exe%" --file="%premake_entry%" --llvm-root="%LLVM_ROOT%" codelite
"%premake_exe%" --file="%premake_entry%" --llvm-root="%LLVM_ROOT%" codeblocks
"%premake_exe%" --file="%premake_entry%" --llvm-root="%LLVM_ROOT%" gmake
@rem "%premake_exe%" --file="%premake_entry%" --llvm-root="%LLVM_ROOT%" vs2005

@pause
