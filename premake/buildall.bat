@set premake_exe=Premake4.exe
@set premake_entry=premake4.lua

"%premake_exe%" --file="%premake_entry%" codelite
"%premake_exe%" --file="%premake_entry%" codeblocks
"%premake_exe%" --file="%premake_entry%" gmake
@rem "%premake_exe%" --file="%premake_entry%" vs2005

@pause
