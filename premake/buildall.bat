@set premake_exe=premake5.exe
@set premake_entry=premake5.lua

"%premake_exe%" --file="%premake_entry%" codelite
"%premake_exe%" --file="%premake_entry%" codeblocks
"%premake_exe%" --file="%premake_entry%" gmake
@rem "%premake_exe%" --file="%premake_entry%" vs2019

@pause
