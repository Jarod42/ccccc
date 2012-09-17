
#include "UnitTest++.h"

#include "allstat.h"

#define CHECK_EQUAL_STAT(lhs, rhs)                                                \
	CHECK_EQUAL(lhs.getLineOfCode_blank(), rhs.getLineOfCode_blank());            \
	CHECK_EQUAL(lhs.getLineOfCode_comment(), rhs.getLineOfCode_comment());        \
	CHECK_EQUAL(lhs.getLineOfCode_physic(), rhs.getLineOfCode_physic());          \
	CHECK_EQUAL(lhs.getLineOfCode_program(), rhs.getLineOfCode_program());        \
	CHECK_EQUAL(lhs.getMcCabeCyclomaticNumber(), rhs.getMcCabeCyclomaticNumber());


TEST(FILE_TEST_C)
{
	AllStat stat;
	const std::string filename = "../../../samples/test.c";

	stat.Compute(filename.c_str(), 0, NULL);

	const unsigned int expectedFileCount = 1;
	CHECK_EQUAL(expectedFileCount, stat.getFileCount());
	const FileStat& fileStat = stat.getFileStat(0);
	LocalStat expectedFileStat(24, 13, 4, 8, 2);
	CHECK_EQUAL_STAT(expectedFileStat, fileStat.getStat());
	CHECK_EQUAL(filename, fileStat.getFilename());

	const unsigned int expectedFuncCount = 1;
	CHECK_EQUAL(expectedFuncCount, fileStat.getFunctionCount());

	const FuncStat& funcStat = fileStat.getFuncStat(0);
	LocalStat expectedFuncStat(11, 9, 1, 2, 2);
	CHECK_EQUAL_STAT(expectedFuncStat, funcStat.getStat());

	const FuncStat* funcStatName = fileStat.getFuncStatByName("main(int, char **)");
	CHECK_EQUAL(&funcStat, funcStatName);
}

TEST(FILE_TEST_H)
{
	AllStat stat;
	const std::string filename = "../../../samples/test.h";

	stat.Compute(filename.c_str(), 0, NULL);

	unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const FileStat& fileStat = stat.getFileStat(0);
	LocalStat expectedStat(12, 8, 4, 0, 1);
	CHECK_EQUAL_STAT(expectedStat, fileStat.getStat());
	CHECK_EQUAL(filename, fileStat.getFilename());
}

TEST(FILE_TEST_INCLUDE_CPP)
{
	const char* argv[] = {"-I../../../samples"};
	AllStat stat;
	const std::string filename = "../../../samples/test_include.cpp";

	stat.Compute(filename.c_str(), 1, argv);

	unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const FileStat& fileStat = stat.getFileStat(0);
	LocalStat expectedStat(23, 14, 4, 6, 2);
	CHECK_EQUAL_STAT(expectedStat, fileStat.getStat());
	CHECK_EQUAL(filename, fileStat.getFilename());
}
