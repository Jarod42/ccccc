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

#include <UnitTest++/UnitTest++.h>
#include <iostream>

namespace
{

void displayFuncNamesList(const ccccc::FileStat& fileStat)
{
	for (std::size_t i = 0; i != fileStat.getFunctionCount(); ++i) {
		auto& funcStat = fileStat.getFuncStat(i);

		std::cerr << funcStat.getName() << std::endl;
	}
}

void displayClassNamesList(const ccccc::FileStat& fileStat)
{
	std::cerr << "Classes are:" << std::endl;
	for (const auto& [className, _] : fileStat.getClasses()) {
		std::cerr << className << std::endl;
	}
}

void displayFuncNamesList(const ccccc::ClassStat& classStat)
{
	std::cerr << "Methods of " << classStat.getName() << " are:" << std::endl;
	for (std::size_t i = 0; i != classStat.getMethodCount(); ++i) {
		auto& funcStat = classStat.getMethodStat(i);

		std::cerr << funcStat.getName() << std::endl;
	}
}

unsigned int getCallerCount(const ccccc::FileStat& fileStat, const char* funcName)
{
	const ccccc::FuncStat* funcStat = fileStat.getFuncStatByName(funcName);

	if (funcStat == nullptr) {
		displayFuncNamesList(fileStat);
		return static_cast<unsigned>(-1);
	}
	return funcStat->getCallerCount();
}

unsigned int
getCallerCount(const ccccc::FileStat& fileStat, const char* className, const char* funcName)
{
	auto* classStat = fileStat.getClassByName(className);

	if (classStat == nullptr) {
		displayClassNamesList(fileStat);
		return static_cast<unsigned>(-1);
	}
	const ccccc::FuncStat* funcStat = classStat->getMethodStatByName(funcName);
	if (funcStat == nullptr) {
		displayFuncNamesList(*classStat);
		return static_cast<unsigned>(-1);
	}
	return funcStat->getCallerCount();
}
} // namespace

TEST(CALLER_COUNT_FILE)
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::string filename = "../../../samples/callercount.cpp";

	param.AddExtra("-std=c++17");
	param.AddFile(filename);
	stat.Compute(param);

	const unsigned int expected = 1;
	CHECK_EQUAL(expected, stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);

	CHECK_EQUAL(2, getCallerCount(fileStat, "ADL(void *)"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "ADL(S *)"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "f<>(S &)"));
	CHECK_EQUAL(0, getCallerCount(fileStat, "f(T &)"));
	CHECK_EQUAL(0, getCallerCount(fileStat, "callADL(S &)"));

	//CHECK_EQUAL(1, getCallerCount(fileStat, "Base", "f1()"));
	//CHECK_EQUAL(0, getCallerCount(fileStat, "Base", "f2()"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "Base", "f3()"));
	CHECK_EQUAL(0, getCallerCount(fileStat, "Base", "f4()"));
	CHECK_EQUAL(0, getCallerCount(fileStat, "Derived", "f1()"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "Derived", "f2()"));
	CHECK_EQUAL(0, getCallerCount(fileStat, "Derived", "f3()"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "Derived", "f4()"));

	CHECK_EQUAL(0, getCallerCount(fileStat, "UseOperator(Operator &)"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "Operator", "operator int()"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "Operator", "operator*()"));

	CHECK_EQUAL(1, getCallerCount(fileStat, "call(void (*)())"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "someFunc()"));
	CHECK_EQUAL(0, getCallerCount(fileStat, "callSomeFunc()"));

	// implicit destructors call doesn't count
	//CHECK_EQUAL(1, getCallerCount(fileStat, "Implicit", "Implicit()"));
	//CHECK_EQUAL(1, getCallerCount(fileStat, "Implicit", "Implicit(const Implicit &)"));
	//CHECK_EQUAL(1, getCallerCount(fileStat, "Implicit", "~Implicit()"));
	//CHECK_EQUAL(1, getCallerCount(fileStat, "DefaultCount", "DefaultCount()"));
	//CHECK_EQUAL(1, getCallerCount(fileStat, "DefaultCount", "DefaultCount(const DefaultCount &)"));
	//CHECK_EQUAL(1, getCallerCount(fileStat, "DefaultCount", "~DefaultCount()"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "UserProvided", "UserProvided()"));
	CHECK_EQUAL(1, getCallerCount(fileStat, "UserProvided", "UserProvided(const UserProvided &)"));
	//CHECK_EQUAL(1, getCallerCount(fileStat, "UserProvided", "~UserProvided()"));

	CHECK_EQUAL(1, getCallerCount(fileStat, "New", "New()"));
	//CHECK_EQUAL(1, getCallerCount(fileStat, "New", "~New()"));

	CHECK_EQUAL(4, getCallerCount(fileStat, "parameter()"));

	CHECK_EQUAL(1, getCallerCount(fileStat, "initialize_global()"));

	CHECK_EQUAL(0, getCallerCount(fileStat, "main()"));
}
