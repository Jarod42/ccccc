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

#include "UnitTest++.h"

#include "allstat.h"
#include "parameters.h"
#include "classstat.h"

#define CHECK_EQUAL_STAT(lhs, rhs)                                             \
	CHECK_EQUAL((lhs).getLineOfCode_blank(), (rhs).getLineOfCode_blank());     \
	CHECK_EQUAL((lhs).getLineOfCode_comment(), (rhs).getLineOfCode_comment()); \
	CHECK_EQUAL((lhs).getLineOfCode_physic(), (rhs).getLineOfCode_physic());   \
	CHECK_EQUAL((lhs).getLineOfCode_program(), (rhs).getLineOfCode_program());


void InitHardCodedMingwPath(ccccc::Parameters& param)
{
	// Hard coded system headersHard coded
#define MINGWPATH "d:/Programs/mingw-4.6.1"
#define MINGW_LIB_PATH "/lib/gcc/mingw32/4.6.1"

	param.AddInclude(MINGWPATH "/include");
	param.AddInclude(MINGWPATH MINGW_LIB_PATH "/include/c++");
	param.AddInclude(MINGWPATH MINGW_LIB_PATH "/include/c++/mingw32");
	param.AddInclude(MINGWPATH MINGW_LIB_PATH "/include/c++/backward");
	param.AddInclude(MINGWPATH MINGW_LIB_PATH "/include");
	param.AddInclude(MINGWPATH MINGW_LIB_PATH "/include-fixed");
}

TEST(LINECOUNT_FILE_TEST_C)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	InitHardCodedMingwPath(param);
	const std::string filename = "../../../samples/linecount/test.c";
	param.AddFile(filename);

	stat.Compute(param);

	const unsigned int expectedFileCount = 1;
	CHECK_EQUAL(expectedFileCount, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedFileStat(24, 13, 4, 8);
	CHECK_EQUAL_STAT(expectedFileStat, fileStat.getLineCount());
	CHECK_EQUAL(filename, fileStat.getFilename());

	const unsigned int expectedFuncCount = 1;
	CHECK_EQUAL(expectedFuncCount, fileStat.getFunctionCount());

	const ccccc::FuncStat& funcStat = fileStat.getFuncStat(0);
	ccccc::LineCount expectedFuncStat(11, 9, 1, 2);
	const unsigned int expectedMvg = 2;
	CHECK_EQUAL_STAT(expectedFuncStat, funcStat.getLineCount());
	CHECK_EQUAL(expectedMvg, funcStat.getMcCabeCyclomaticNumber());

	const ccccc::FuncStat* funcStatName = fileStat.getFuncStatByName("main(int, char **)");
	CHECK_EQUAL(&funcStat, funcStatName);
}

TEST(LINECOUNT_FILE_TEST_H)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	InitHardCodedMingwPath(param);
	const std::string filename = "../../../samples/linecount/test.h";

	param.AddFile(filename);
	stat.Compute(param);

	unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedStat(12, 8, 4, 0);
	CHECK_EQUAL_STAT(expectedStat, fileStat.getLineCount());
	CHECK_EQUAL(filename, fileStat.getFilename());
}

TEST(LINECOUNT_FILE_TEST_INCLUDE_CPP)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	InitHardCodedMingwPath(param);
	const std::string filename = "../../../samples/linecount/test_include.cpp";

	param.AddInclude("../../../samples/linecount");
	//param.AddExtra("-std=c++0x");
	param.AddFile(filename);
	stat.Compute(param);

	unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedStat(23, 14, 4, 6);
	CHECK_EQUAL_STAT(expectedStat, fileStat.getLineCount());
	CHECK_EQUAL(filename, fileStat.getFilename());
}

TEST(FILE_TEST_NAMESPACE_CPP)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	InitHardCodedMingwPath(param);
	const std::string filename = "../../../samples/namespace.cpp";

	//param.AddInclude("../../../samples");
	//param.AddExtra("-std=c++0x");
	param.AddFile(filename);
	stat.Compute(param);

	unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedStat(4, 4, 0, 0);
	const unsigned int expectedMvg = 1;
	CHECK(fileStat.getFuncStatByName("sum(int, int)") == NULL);
	const ccccc::NamespaceStat* namespaceStat = fileStat.getNamespaceByName("Foo");
	CHECK(namespaceStat != NULL);
	const ccccc::FuncStat* funcStat = namespaceStat->getFuncStatByName("sum(int, int)");
	CHECK(funcStat != NULL);
	CHECK_EQUAL_STAT(expectedStat, funcStat->getLineCount());
	CHECK_EQUAL(expectedMvg, funcStat->getMcCabeCyclomaticNumber());
}

TEST(FILE_TEST_CLASS_CPP)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	InitHardCodedMingwPath(param);
	const std::string filename = "../../../samples/class.cpp";

	//param.AddInclude("../../../samples");
	param.AddExtra("-std=c++0x");
	param.AddFile(filename);
	stat.Compute(param);

	unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);

	const ccccc::ClassStat* classFooStat = fileStat.getClassByName("Foo");
	CHECK(classFooStat != NULL);
	CHECK(classFooStat->getMethodCount() == 4);
	CHECK(classFooStat->getMethodStatByName("Foo()") != NULL);
	CHECK(classFooStat->getMethodStatByName("Foo(Foo &&)") != NULL);
	CHECK(classFooStat->getMethodStatByName("bar()") != NULL);
	CHECK(classFooStat->getMethodStatByName("isNul()") != NULL);

	CHECK(classFooStat->getClassCount() == 1);
	const ccccc::ClassStat* classInnerStat = classFooStat->getClassByName("InnerClass");
	CHECK(classInnerStat != NULL);
	CHECK(classInnerStat->getMethodStatByName("InnerClass()") != NULL);
	CHECK(classInnerStat->getMethodStatByName("~InnerClass()") != NULL);
	CHECK(classInnerStat->getMethodStatByName("bar()") == NULL);
}
