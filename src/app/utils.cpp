

#include "utils.h"

#ifdef _WIN32

#include <windows.h>
#include <libgen.h>

std::string getExePath()
{
	char ownPth[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);

	if (hModule != NULL) {
		GetModuleFileName(hModule, ownPth, sizeof (ownPth));
	}
	return dirname(ownPth);
}

#else

//Mac OS X: _NSGetExecutablePath() (man 3 dyld)
//Linux: readlink /proc/self/exe
//Solaris: getexecname()
//FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
//BSD with procfs: readlink /proc/curproc/file

std::string getExePath()
{
#warning "platform not supported: getExePath will return empty"
	return std::string();
}

#endif

