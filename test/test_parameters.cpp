
#include <UnitTest++.h>

#include "parameters.h"

#define ARRAY_SIZE(a) (sizeof (a) / sizeof (*a))

template <typename Iterator1, typename Iterator2>
bool IsRangeEq(Iterator1 begin1, Iterator1 end1, Iterator2 begin2, Iterator2 end2)
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
	Parameters param;

	for (int i = 0; i != ARRAY_SIZE(files); ++i) {
		param.AddFile(files[i]);
	}
	CHECK(IsRangeEq(files, files + ARRAY_SIZE(files), param.Files_begin(), param.Files_end()));
}

TEST(PARAMETERS_ADD_INCLUDE)
{
	std::string includes[] = {"/usr/include", "./", "c:\\foo"};
	Parameters param;

	for (int i = 0; i != ARRAY_SIZE(includes); ++i) {
		param.AddInclude(includes[i]);
	}
	CHECK(IsRangeEq(includes, includes + ARRAY_SIZE(includes), param.IncludePaths_begin(), param.IncludePaths_end()));
}

TEST(PARAMETERS_ADD_DEFINE)
{
	std::string defines[] = {"FOO", "BAR=42"};
	Parameters param;

	for (int i = 0; i != ARRAY_SIZE(defines); ++i) {
		param.AddDefine(defines[i]);
	}
	CHECK(IsRangeEq(defines, defines + ARRAY_SIZE(defines), param.Defines_begin(), param.Defines_end()));
}

TEST(PARAMETERS_ADD_EXTRA)
{
	std::string extras[] = {"-std=c++0x"};
	Parameters param;

	for (int i = 0; i != ARRAY_SIZE(extras); ++i) {
		param.AddExtra(extras[i]);
	}
	CHECK(IsRangeEq(extras, extras + ARRAY_SIZE(extras), param.Extras_begin(), param.Extras_end()));
}

TEST(PARAMETERS_ADD_PCH)
{
	std::string pchFile = "pchFile";
	Parameters param;

	param.SetPch(pchFile);
	CHECK_EQUAL(pchFile, param.GetPch());
}
