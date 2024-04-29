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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mstch/mstch.hpp>
#include <sstream>

namespace
{

std::string getFileContent(const std::filesystem::path& path)
{
	std::ifstream file(path);
	return {std::istream_iterator<char>(file), std::istream_iterator<char>()};
}

// Default bin location is Root/bin/$IDE/$Config
std::filesystem::path getRootPath(const std::filesystem::path& exePath)
{
	auto res = exePath.parent_path().parent_path();
	if (res.filename() != "bin") {
		return exePath;
	}
	return res.parent_path();
}

std::string getLocalTime()
{
	time_t now;
	time(&now);

	std::tm timeinfo;
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
	localtime_s(&timeinfo, &now);
#else // POSIX
	localtime_r(&now, &timeinfo);
#endif

	char formattedDate[32]; // as "Thu Aug 23 14:55:02 2001"
	strftime(formattedDate, sizeof(formattedDate), "%c", &timeinfo);

	return formattedDate;
}

} // namespace

int main(int argc, char* argv[])
{
	const std::filesystem::path cccccPath = getExePath();
	const std::filesystem::path cccccRoot = getRootPath(cccccPath);
	ccccc::Parameters params;

	params.Parse(cccccRoot, argc, argv);
	ccccc::AllStat allStat;

	allStat.Compute(params);

	mstch::map dict;
	const std::filesystem::path templateFilename = params.GetTemplateFilename();

	dict["cccccPath"] = cccccPath.string();
	dict["cccccRoot"] = cccccRoot.string();
	dict["Date"] = getLocalTime();
	const std::filesystem::path root = std::filesystem::current_path();
	for (std::size_t i = 0; i != allStat.getFileCount(); ++i) {
		const ccccc::FileStat& filestat = allStat.getFileStat(i);
		std::cerr << "feed dict: " << filestat.getFilename() << std::endl;

		feedDict(filestat, root, &dict);
	}

	std::cout << mstch::render(getFileContent(templateFilename), dict);
	return 0;
}
