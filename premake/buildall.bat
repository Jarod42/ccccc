@set premake_exe=premake5.exe
@set premake_entry=premake5.lua

"%premake_exe%" --file="%premake_entry%" codelite --without-tinfo --cc=gcc
"%premake_exe%" --file="%premake_entry%" gmake2 --without-tinfo
@rem "%premake_exe%" --file="%premake_entry%" vs2022

@pause
