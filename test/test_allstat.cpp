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

#include "allstat.h"
#include "classstat.h"
#include "filestat.h"
#include "linecount.h"
#include "parameters.h"

#include <UnitTest++/UnitTest++.h>

#define CHECK_EQUAL_LOC(lhs, rhs) \
 CHECK_EQUAL((lhs).getLineOfCode_blank(), (rhs).getLineOfCode_blank()); \
 CHECK_EQUAL((lhs).getLineOfCode_comment(), (rhs).getLineOfCode_comment()); \
 CHECK_EQUAL((lhs).getLineOfCode_physic(), (rhs).getLineOfCode_physic()); \
 CHECK_EQUAL((lhs).getLineOfCode_program(), (rhs).getLineOfCode_program());

TEST(LINECOUNT_FILE_TEST_C)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/linecount/test.c";
	param.AddFile(filename);

	stat.Compute(param);

	const unsigned int expectedFileCount = 1;
	CHECK_EQUAL(expectedFileCount, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedFileStat(23, 12, 3, 8);
	CHECK_EQUAL_LOC(expectedFileStat, fileStat.getLineCount());
	CHECK_EQUAL(std::filesystem::absolute(filename), fileStat.getFilename());

	const unsigned int expectedFuncCount = 1;
	CHECK_EQUAL(expectedFuncCount, fileStat.getFunctionCount());

	const ccccc::FuncStat& funcStat = fileStat.getFuncStat(0);
	ccccc::LineCount expectedFuncStat(11, 9, 1, 2);
	const unsigned int expectedMvg = 2;
	CHECK_EQUAL_LOC(expectedFuncStat, funcStat.getLineCount());
	CHECK_EQUAL(expectedMvg, funcStat.getMcCabeCyclomaticNumber());

	const ccccc::FuncStat* funcStatName = fileStat.getFuncStatByName("main(int, char **)");
	CHECK_EQUAL(&funcStat, funcStatName);
}

TEST(LINECOUNT_FILE_TEST_H)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/linecount/test.h";

	param.AddFile(filename);
	stat.Compute(param);

	const unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedStat(13, 8, 4, 0);
	CHECK_EQUAL_LOC(expectedStat, fileStat.getLineCount());
	CHECK_EQUAL(std::filesystem::absolute(filename), fileStat.getFilename());
}

TEST(LINECOUNT_FILE_TEST_INCLUDE_CPP)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/linecount/test_include.cpp";

	param.AddInclude("../../../samples/linecount");
	//param.AddExtra("-std=c++11");
	param.AddFile(filename);
	stat.Compute(param);

	const unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedStat(24, 14, 4, 6);
	CHECK_EQUAL_LOC(expectedStat, fileStat.getLineCount());
	CHECK_EQUAL(std::filesystem::absolute(filename), fileStat.getFilename());
}

TEST(FILE_TEST_NAMESPACE_CPP)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/namespace.cpp";

	//param.AddInclude("../../../samples");
	//param.AddExtra("-std=c++11");
	param.AddFile(filename);
	stat.Compute(param);

	const unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedStat(4, 4, 0, 0);
	const unsigned int expectedMvg = 1;
	CHECK(fileStat.getFuncStatByName("sum(int, int)") == nullptr);
	const ccccc::NamespaceStat* namespaceStat = fileStat.getNamespaceByName("Foo");
	CHECK(namespaceStat != nullptr);
	const ccccc::FuncStat* funcStat = namespaceStat->getFuncStatByName("sum(int, int)");
	CHECK(funcStat != nullptr);
	CHECK_EQUAL_LOC(expectedStat, funcStat->getLineCount());
	CHECK_EQUAL(expectedMvg, funcStat->getMcCabeCyclomaticNumber());
}

TEST(FILE_TEST_CLASS_CPP)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/class.cpp";

	//param.AddInclude("../../../samples");
	param.AddExtra("-std=c++11");
	param.AddFile(filename);
	stat.Compute(param);

	const unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);

	const ccccc::ClassStat* classFooStat = fileStat.getClassByName("Foo");
	CHECK(classFooStat != nullptr);
	CHECK(classFooStat->getMethodCount() == 4);
	CHECK(classFooStat->getMethodStatByName("Foo()") != nullptr);
	CHECK(classFooStat->getMethodStatByName("Foo(Foo &&)") != nullptr);
	CHECK(classFooStat->getMethodStatByName("bar()") != nullptr);
	CHECK(classFooStat->getMethodStatByName("isNul()") != nullptr);

	CHECK(classFooStat->getClassCount() == 1);
	const ccccc::ClassStat* classInnerStat = classFooStat->getClassByName("InnerClass");
	CHECK(classInnerStat != nullptr);
	CHECK(classInnerStat->getMethodStatByName("InnerClass()") != nullptr);
	CHECK(classInnerStat->getMethodStatByName("~InnerClass()") != nullptr);
	CHECK(classInnerStat->getMethodStatByName("bar()") == nullptr);
}

static bool CheckMvg(const ccccc::FileStat& fileStat, const char* funcName, unsigned expectedMvg)
{
	const ccccc::FuncStat* funcStat = fileStat.getFuncStatByName(funcName);

	if (funcStat == nullptr) {
		return false;
	}
	return expectedMvg == funcStat->getMcCabeCyclomaticNumber();
}

TEST(FILE_TEST_MVG_CPP)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/mvg.cpp";

	//param.AddInclude("../../../samples");
	param.AddExtra("-std=c++0x");
	param.AddFile(filename);
	stat.Compute(param);

	const unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);

	CHECK(CheckMvg(fileStat, "function_comparaison(int, int)", 1));
	CHECK(CheckMvg(fileStat, "function_if(int, int)", 2));
	CHECK(CheckMvg(fileStat, "function_ifelse(int, int)", 2));
	CHECK(CheckMvg(fileStat, "function_ternaire(int, int)", 2));
	CHECK(CheckMvg(fileStat, "function_while(const char *)", 2));
	CHECK(CheckMvg(fileStat, "function_for(const char *)", 2));
	CHECK(CheckMvg(fileStat, "function_switch(int)", 8));
	CHECK(CheckMvg(fileStat, "function_and(int, int, int)", 1));
	CHECK(CheckMvg(fileStat, "function_andand(int, int, int)", 2));
	CHECK(CheckMvg(fileStat, "function_or(int, int, int)", 1));
	CHECK(CheckMvg(fileStat, "function_oror(int, int, int)", 2));
	CHECK(CheckMvg(fileStat, "function_rval(T &, T &)", 1));
	CHECK(CheckMvg(fileStat, "function_rval(int &&)", 1));
}

static int CheckBlockCount(const ccccc::FileStat& fileStat, const char* funcName)
{
	const ccccc::FuncStat* funcStat = fileStat.getFuncStatByName(funcName);

	if (funcStat == nullptr) {
		return -1;
	}
	return funcStat->getNestedBlockCount();
}

TEST(FILE_TEST_BLOCKCOUNT_CPP)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/blockcount.cpp";

	//param.AddInclude("../../../samples");
	//param.AddExtra("-std=c++0x");
	param.AddFile(filename);
	stat.Compute(param);

	const unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);

	CHECK_EQUAL(0, CheckBlockCount(fileStat, "function_comparaison(int, int)"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_if(int, int)"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_ifelse(int, int)"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_ifelif(int, int, int)"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_ifif(int, int, int)"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_while(const char *)"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_for_noblock(const char *)"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_forfor(const char (&)[5][5])"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_forfor_no_block(const char (&)[5][5])"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_switch(int)"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_block()"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_max_block()"));
}
