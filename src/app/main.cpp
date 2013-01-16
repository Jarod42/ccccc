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

#include <iostream>
#include <sstream>

#include "cccc_clang_api.h"
#include "feeddict.h"
#include "getexepath.h"

#include <ctemplate/template.h>

std::string normalizePath(const std::string& path)
{
	std::string res = path;

	for (std::string::size_type t = res.find("\\"); t != std::string::npos; t = res.find("\\", t)) {
		res[t] = '/';
	}
	return res;
}

// Default bin location is Root/bin/$IDE/$Config
std::string getRootPath(const std::string& exePath)
{
	std::string::size_type t = exePath.rfind("/");
	for (int i = 0; i != 2; ++i) {
		if (t == std::string::npos) {
			return exePath;
		}
		t = exePath.rfind("/", t - 1);
	}
	if (exePath.compare(t + 1, 4, "bin/") != 0) {
		return exePath;
	}
	std::string res = exePath.substr(0, t);
	return res;
}

int main(int argc, char* argv[])
{
	ccccc::Parameters params;

	params.Parse(argc, argv);
	ccccc::AllStat allStat;

	allStat.Compute(params);

	const std::string cccccPath = normalizePath(getExePath());
	const std::string cccccRoot = getRootPath(cccccPath);
	ctemplate::TemplateDictionary dict("root");
	const std::string templateFilename = cccccRoot + "/template/html/template.tpl";

	dict.SetFilename(templateFilename);
	dict.SetValue("cccccPath", cccccPath);
	dict.SetValue("cccccRoot", cccccRoot);
	{
		time_t now;
		time(&now);
		dict.SetValue("Date", ctime(&now));
	}
	for (unsigned int i = 0; i != allStat.getFileCount(); ++i) {
		const ccccc::FileStat& filestat = allStat.getFileStat(i);

		feedDict(filestat, &dict);
	}

	std::string output;
	ctemplate::ExpandTemplate(templateFilename, ctemplate::DO_NOT_STRIP, &dict, &output);
	std::cout << output;
	return 0;
}
