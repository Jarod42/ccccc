/*
** Copyright 2012-2022 Joris Dauphin
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

#include "cccc_clang_api.h"
#include "feeddict.h"
#include "getexepath.h"

#include <ctemplate/template.h>
#include <filesystem>
#include <iostream>
#include <sstream>

// Default bin location is Root/bin/$IDE/$Config
std::filesystem::path getRootPath(const std::filesystem::path& exePath)
{
	auto res = exePath.parent_path().parent_path();
	if (res.filename() != "bin") {
		return exePath;
	}
	return res.parent_path();
}

int main(int argc, char* argv[])
{
	const std::filesystem::path cccccPath = getExePath();
	const std::filesystem::path cccccRoot = getRootPath(cccccPath);
	ccccc::Parameters params;

	params.Parse(cccccRoot, argc, argv);
	ccccc::AllStat allStat;

	allStat.Compute(params);

	ctemplate::TemplateDictionary dict("root");
	const std::filesystem::path templateFilename = params.GetTemplateFilename();

	dict.SetFilename(templateFilename.string());
	dict.SetValue("cccccPath", cccccPath.string());
	dict.SetValue("cccccRoot", cccccRoot.string());
	{
		time_t now;
		time(&now);
		dict.SetValue("Date", ctime(&now));
	}
	const std::filesystem::path root = std::filesystem::current_path();
	for (unsigned int i = 0; i != allStat.getFileCount(); ++i) {
		const ccccc::FileStat& filestat = allStat.getFileStat(i);
		std::cerr << "feed dict: " << filestat.getFilename() << std::endl;

		feedDict(filestat, root, &dict);
	}

	std::string output;
	ctemplate::ExpandTemplate(templateFilename.string(), ctemplate::DO_NOT_STRIP, &dict, &output);
	std::cout << output;
	return 0;
}
