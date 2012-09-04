
#include "UnitTest++.h"

#include "cccc_clang_api.h"
#include "localstat.h"


TEST(FILE_TEST_C)
{
	std::vector<LocalStat> localStats;

	ComputeStats("../../../samples/test.c", 0, NULL, localStats);

	const size_t expected = 1 + 1;
	CHECK_EQUAL(expected, localStats.size());
}


TEST(FILE_TEST_H)
{
	std::vector<LocalStat> localStats;

	ComputeStats("../../../samples/test.h", 0, NULL, localStats);

	const size_t expected = 1 + 1;
	CHECK_EQUAL(expected, localStats.size());
}

#include <iostream>

TEST(FILE_TEST_INCLUDE_CPP)
{
	std::vector<LocalStat> localStats;
	const char* argv[] = {"-I../../../samples"};

	ComputeStats("../../../samples/test_include.cpp", 1, argv, localStats);

	const size_t expected = 1 + 1;
	CHECK_EQUAL(expected, localStats.size());
}


