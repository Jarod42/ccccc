/*
** Copyright 2012-2015 Joris Dauphin
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

#include <UnitTest++.h>

#include "parameters.h"

template <typename T, std::size_t N>
static constexpr std::size_t ArraySize(const T (&)[N]) { return N; }

TEST(PARAMETERS_ADD_FILE)
{
	const std::vector<std::string> files{"a.c", "a.h", "b.c", "b.h"};
	ccccc::Parameters param;

	for (const auto& file : files) {
		param.AddFile(file);
	}
	CHECK(files == param.Filenames());
}

TEST(PARAMETERS_ADD_INCLUDE)
{
	const std::vector<std::string> includes{"/usr/include", "./", "c:\\foo"};
	ccccc::Parameters param;

	for (const auto& include : includes) {
		param.AddInclude(include);
	}
	CHECK(includes == param.IncludePaths());
}

TEST(PARAMETERS_ADD_DEFINE)
{
	const std::vector<std::string> defines{"FOO", "BAR=42"};
	ccccc::Parameters param;

	for (const auto& define : defines) {
		param.AddDefine(define);
	}
	CHECK(defines == param.Defines());
}

TEST(PARAMETERS_ADD_EXTRA)
{
	const std::vector<std::string> extras{"-std=c++0x"};
	ccccc::Parameters param;

	for (const auto& extra : extras) {
		param.AddExtra(extra);
	}
	CHECK(extras == param.Extras());
}

TEST(PARAMETERS_ADD_PCH)
{
	std::string pchFile = "pchFile";
	ccccc::Parameters param;

	param.SetPch(pchFile);
	CHECK_EQUAL(pchFile, param.GetPch());
}

TEST(PARAMETERS_PARSING_SHORT_OPTIONS)
{
	const char* argv0 = "ccccc_test";
	const char* includeFlag = "-I";
	const char* defineFlag = "-D";
	const char* extraFlag = "-e";
	const char* pchFlag = "-p";
	const std::vector<std::string> files{"a.c", "a.h"};
	const std::vector<std::string> includes{"includeDir1", "includeDir2"};
	const std::vector<std::string> defines{"FOO", "BAR=42"};
	const std::vector<std::string> extras{"-std=c++0x"};
	std::string pchFile = "pchFile";
	const char* argv[] = {argv0,
						  includeFlag, includes[0].c_str(), defineFlag, defines[0].c_str(), extraFlag, extras[0].c_str(),
						  pchFlag, pchFile.c_str(), defineFlag, defines[1].c_str(), includeFlag, includes[1].c_str(),
						  files[0].c_str(), files[1].c_str()
						 };
	ccccc::Parameters param;

	param.Parse(ArraySize(argv), const_cast<char**>(argv));

	CHECK(files == param.Filenames());
	CHECK(includes == param.IncludePaths());
	CHECK(defines == param.Defines());
	CHECK(extras == param.Extras());
	CHECK_EQUAL(pchFile, param.GetPch());
}

TEST(PARAMETERS_PARSING_LONG_OPTIONS)
{
	const char* argv0 = "ccccc_test";
	const char* includeFlag = "--include-dir";
	const char* defineFlag = "--define";
	const char* extraFlag = "--extra-option";
	const char* pchFlag = "--pch";
	const std::vector<std::string> files{"a.c", "a.h"};
	const std::vector<std::string> includes{"includeDir1", "includeDir2"};
	const std::vector<std::string> defines{"FOO", "BAR=42"};
	const std::vector<std::string> extras{"-std=c++0x"};
	std::string pchFile = "pchFile";
	const char* argv[] = {argv0,
						  includeFlag, includes[0].c_str(), defineFlag, defines[0].c_str(), extraFlag, extras[0].c_str(),
						  pchFlag, pchFile.c_str(), defineFlag, defines[1].c_str(), includeFlag, includes[1].c_str(),
						  files[0].c_str(), files[1].c_str()
						 };
	ccccc::Parameters param;

	param.Parse(ArraySize(argv), const_cast<char**>(argv));

	CHECK(files == param.Filenames());
	CHECK(includes == param.IncludePaths());
	CHECK(defines == param.Defines());
	CHECK(extras == param.Extras());
	CHECK_EQUAL(pchFile, param.GetPch());
}
