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

#include "parameters.h"

#include <doctest.h>

TEST_CASE("PARAMETERS_ADD_FILE")
{
	const std::vector<std::filesystem::path> files{"a.c", "a.h", "b.c", "b.h"};
	ccccc::Parameters param;

	for (const auto& file : files) {
		param.AddFile(file);
	}
	CHECK(files == param.Filenames());
}

TEST_CASE("PARAMETERS_ADD_INCLUDE")
{
	const std::vector<std::string> includes{"/usr/include", "./", "c:\\foo"};
	ccccc::Parameters param;

	for (const auto& include : includes) {
		param.AddInclude(include);
	}
	CHECK(includes == param.IncludePaths());
}

TEST_CASE("PARAMETERS_ADD_DEFINE")
{
	const std::vector<std::string> defines{"FOO", "BAR=42"};
	ccccc::Parameters param;

	for (const auto& define : defines) {
		param.AddDefine(define);
	}
	CHECK(defines == param.Defines());
}

TEST_CASE("PARAMETERS_ADD_EXTRA")
{
	const std::vector<std::string> extras{"-std=c++14"};
	ccccc::Parameters param;

	for (const auto& extra : extras) {
		param.AddExtra(extra);
	}
	CHECK(extras == param.Extras());
}

TEST_CASE("PARAMETERS_ADD_PCH")
{
	std::string pchFile = "pchFile";
	ccccc::Parameters param;

	param.SetPch(pchFile);
	CHECK(pchFile == param.GetPch());
}

TEST_CASE("PARAMETERS_PARSING_SHORT_OPTIONS")
{
	const std::string cccccRoot = ".";
	const char* argv0 = "ccccc_test";
	const char* includeFlag = "-I";
	const char* defineFlag = "-D";
	const char* extraFlag = "-e";
	const char* pchFlag = "-pch";
	const char* templateFlag = "-t";
	const std::vector<std::filesystem::path> files{"a.c", "a.h"};
	const std::vector<std::string> stringFiles{files[0].string(), files[1].string()};
	const std::vector<std::string> includes{"includeDir1", "includeDir2"};
	const std::vector<std::string> defines{"FOO", "BAR=42"};
	const std::vector<std::string> extras{"-std=c++14"};
	std::string pchFile = "pchFile";
	std::string templateFile = "templateFile";
	const char* argv[] = {argv0,
	                      includeFlag,
	                      includes[0].c_str(),
	                      defineFlag,
	                      defines[0].c_str(),
	                      extraFlag,
	                      extras[0].c_str(),
	                      pchFlag,
	                      pchFile.c_str(),
	                      defineFlag,
	                      defines[1].c_str(),
	                      includeFlag,
	                      includes[1].c_str(),
	                      templateFlag,
	                      templateFile.c_str(),
	                      stringFiles[0].c_str(),
	                      stringFiles[1].c_str()};
	ccccc::Parameters param;

	param.Parse(cccccRoot, std::size(argv), const_cast<char**>(argv));

	CHECK(files == param.Filenames());
	CHECK(includes == param.IncludePaths());
	CHECK(defines == param.Defines());
	CHECK(extras == param.Extras());
	CHECK(pchFile == param.GetPch());
	CHECK(templateFile == param.GetTemplateFilename());
}

TEST_CASE("PARAMETERS_PARSING_LONG_OPTIONS")
{
	const std::string cccccRoot = ".";
	const char* argv0 = "ccccc_test";
	const char* includeFlag = "-include-dir";
	const char* defineFlag = "-define";
	const char* extraFlag = "-extra-option";
	const char* pchFlag = "-pch";
	const char* templateFlag = "-template-file";
	const std::vector<std::filesystem::path> files{"a.c", "a.h"};
	const std::vector<std::string> stringFiles{files[0].string(), files[1].string()};
	const std::vector<std::string> includes{"includeDir1", "includeDir2"};
	const std::vector<std::string> defines{"FOO", "BAR=42"};
	const std::vector<std::string> extras{"-std=c++14"};
	std::string pchFile = "pchFile";
	std::string templateFile = "templateFile";
	const char* argv[] = {argv0,
	                      includeFlag,
	                      includes[0].c_str(),
	                      defineFlag,
	                      defines[0].c_str(),
	                      extraFlag,
	                      extras[0].c_str(),
	                      pchFlag,
	                      pchFile.c_str(),
	                      defineFlag,
	                      defines[1].c_str(),
	                      includeFlag,
	                      includes[1].c_str(),
	                      templateFlag,
	                      templateFile.c_str(),
	                      stringFiles[0].c_str(),
	                      stringFiles[1].c_str()};
	ccccc::Parameters param;

	param.Parse(cccccRoot, std::size(argv), const_cast<char**>(argv));

	CHECK(files == param.Filenames());
	CHECK(includes == param.IncludePaths());
	CHECK(defines == param.Defines());
	CHECK(extras == param.Extras());
	CHECK(pchFile == param.GetPch());
	CHECK(templateFile == param.GetTemplateFilename());
}
