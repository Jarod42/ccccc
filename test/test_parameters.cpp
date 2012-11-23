/*
** Copyright 2012 Joris Dauphin
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

#define ARRAY_SIZE(a) (sizeof (a) / sizeof (*a))

template <typename Iterator1, typename Iterator2>
static bool IsRangeEq(Iterator1 begin1, Iterator1 end1, Iterator2 begin2, Iterator2 end2)
{
	if (end1 - begin1 != end2 - begin2) {
		return false;
	}
	Iterator1 it1 = begin1;
	Iterator2 it2 = begin2;
	for (; it1 != end1; ++it1, ++it2) {
		if (*it1 != *it2) {
			return false;
		}
	}
	return true;
}



TEST(PARAMETERS_ADD_FILE)
{
	std::string files[] = {"a.c", "a.h", "b.c", "b.h"};
	ccccc::Parameters param;

	for (int i = 0; i != ARRAY_SIZE(files); ++i) {
		param.AddFile(files[i]);
	}
	CHECK(IsRangeEq(files, files + ARRAY_SIZE(files), param.Files_begin(), param.Files_end()));
}

TEST(PARAMETERS_ADD_INCLUDE)
{
	std::string includes[] = {"/usr/include", "./", "c:\\foo"};
	ccccc::Parameters param;

	for (int i = 0; i != ARRAY_SIZE(includes); ++i) {
		param.AddInclude(includes[i]);
	}
	CHECK(IsRangeEq(includes, includes + ARRAY_SIZE(includes), param.IncludePaths_begin(), param.IncludePaths_end()));
}

TEST(PARAMETERS_ADD_DEFINE)
{
	std::string defines[] = {"FOO", "BAR=42"};
	ccccc::Parameters param;

	for (int i = 0; i != ARRAY_SIZE(defines); ++i) {
		param.AddDefine(defines[i]);
	}
	CHECK(IsRangeEq(defines, defines + ARRAY_SIZE(defines), param.Defines_begin(), param.Defines_end()));
}

TEST(PARAMETERS_ADD_EXTRA)
{
	std::string extras[] = {"-std=c++0x"};
	ccccc::Parameters param;

	for (int i = 0; i != ARRAY_SIZE(extras); ++i) {
		param.AddExtra(extras[i]);
	}
	CHECK(IsRangeEq(extras, extras + ARRAY_SIZE(extras), param.Extras_begin(), param.Extras_end()));
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
	std::string files[] = {"a.c", "a.h"};
	std::string includes[] = {"includeDir1", "includeDir2"};
	std::string defines[] = {"FOO", "BAR=42"};
	std::string extras[] = {"-std=c++0x"};
	std::string pchFile = "pchFile";
	const char *argv[] = {argv0,
		includeFlag, includes[0].c_str(), defineFlag, defines[0].c_str(), extraFlag, extras[0].c_str(),
		pchFlag, pchFile.c_str(), defineFlag, defines[1].c_str(), includeFlag, includes[1].c_str(),
		files[0].c_str(), files[1].c_str()
	};
	ccccc::Parameters param;

	param.Parse(ARRAY_SIZE(argv), const_cast<char **>(argv));

	CHECK(IsRangeEq(files, files + ARRAY_SIZE(files), param.Files_begin(), param.Files_end()));
	CHECK(IsRangeEq(includes, includes + ARRAY_SIZE(includes), param.IncludePaths_begin(), param.IncludePaths_end()));
	CHECK(IsRangeEq(defines, defines + ARRAY_SIZE(defines), param.Defines_begin(), param.Defines_end()));
	CHECK(IsRangeEq(extras, extras + ARRAY_SIZE(extras), param.Extras_begin(), param.Extras_end()));
	CHECK_EQUAL(pchFile, param.GetPch());
}

TEST(PARAMETERS_PARSING_LONG_OPTIONS)
{
	const char* argv0 = "ccccc_test";
	const char* includeFlag = "--include-dir";
	const char* defineFlag = "--define";
	const char* extraFlag = "--extra-option";
	const char* pchFlag = "--pch";
	std::string files[] = {"a.c", "a.h"};
	std::string includes[] = {"includeDir1", "includeDir2"};
	std::string defines[] = {"FOO", "BAR=42"};
	std::string extras[] = {"-std=c++0x"};
	std::string pchFile = "pchFile";
	const char *argv[] = {argv0,
		includeFlag, includes[0].c_str(), defineFlag, defines[0].c_str(), extraFlag, extras[0].c_str(),
		pchFlag, pchFile.c_str(), defineFlag, defines[1].c_str(), includeFlag, includes[1].c_str(),
		files[0].c_str(), files[1].c_str()
	};
	ccccc::Parameters param;

	param.Parse(ARRAY_SIZE(argv), const_cast<char **>(argv));

	CHECK(IsRangeEq(files, files + ARRAY_SIZE(files), param.Files_begin(), param.Files_end()));
	CHECK(IsRangeEq(includes, includes + ARRAY_SIZE(includes), param.IncludePaths_begin(), param.IncludePaths_end()));
	CHECK(IsRangeEq(defines, defines + ARRAY_SIZE(defines), param.Defines_begin(), param.Defines_end()));
	CHECK(IsRangeEq(extras, extras + ARRAY_SIZE(extras), param.Extras_begin(), param.Extras_end()));
	CHECK_EQUAL(pchFile, param.GetPch());
}
