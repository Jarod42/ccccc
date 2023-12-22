/*
** Copyright 2022 Joris Dauphin
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
#include "utils.h"

#include <doctest.h>

TEST_CASE("FILE_TEST_NAMESPACE_CPP")
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/namespace.cpp";

	//param.AddExtra("-std=c++11");
	param.AddFile(filename);
	stat.Compute(param);

	const std::size_t expected = 1;
	CHECK(expected == stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);
	ccccc::LineCount expectedStat(4, 4, 0, 0);
	const std::size_t expectedMvg = 1;
	CHECK(fileStat.getFuncStatByName("sum(int, int)") == nullptr);
	const ccccc::NamespaceStat* namespaceStat = fileStat.getNamespaceByName("Foo");
	CHECK(namespaceStat != nullptr);
	const ccccc::FuncStat* funcStat = namespaceStat->getFuncStatByName("sum(int, int)");
	CHECK(funcStat != nullptr);
	CHECK_EQUAL_LOC(expectedStat, funcStat->getLineCount());
	CHECK(expectedMvg == funcStat->getMcCabeCyclomaticNumber());
}

TEST_CASE("FILE_TEST_CLASS_CPP")
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::filesystem::path filename = "../../../samples/class.cpp";

	param.AddExtra("-std=c++11");
	param.AddFile(filename);
	stat.Compute(param);

	const std::size_t expected = 1;
	CHECK(expected == stat.getFileCount());
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
