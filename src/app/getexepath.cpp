/*
** Copyright 2012-2013 Joris Dauphin
*/
/*
**  This file is part of CCCCC.
**
**  CCCCC is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  CCCCC is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with CCCCC. If not, see <http://www.gnu.org/licenses/>.
*/

#include "getexepath.h"

#ifdef _WIN32

#include <windows.h>
#include <libgen.h>

std::string getExePath()
{
	char ownPth[MAX_PATH];
	HMODULE hModule = GetModuleHandle(nullptr);

	if (hModule != nullptr) {
		GetModuleFileName(hModule, ownPth, sizeof(ownPth));
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

