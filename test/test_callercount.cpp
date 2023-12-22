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

std::size_t getCallerCount(const ccccc::FileStat& fileStat, const char* funcName)
{
	const ccccc::FuncStat* funcStat = fileStat.getFuncStatByName(funcName);

	if (funcStat == nullptr) {
		displayFuncNamesList(fileStat);
		return static_cast<unsigned>(-1);
	}
	return funcStat->getCallerCount();
}

std::size_t
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

TEST_CASE("CALLER_COUNT_FILE")
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::string filename = "../../../samples/callercount.cpp";

	param.AddExtra("-std=c++17");
	param.AddFile(filename);
	stat.Compute(param);

	const std::size_t expected = 1;
	CHECK(expected == stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);

	CHECK(2 == getCallerCount(fileStat, "ADL(void *)"));
	CHECK(1 == getCallerCount(fileStat, "ADL(S *)"));
	CHECK(1 == getCallerCount(fileStat, "f<>(S &)"));
	CHECK(0 == getCallerCount(fileStat, "f(T &)"));
	CHECK(0 == getCallerCount(fileStat, "callADL(S &)"));

	//CHECK(1 == getCallerCount(fileStat, "Base", "f1()"));
	//CHECK(0 == getCallerCount(fileStat, "Base", "f2()"));
	CHECK(1 == getCallerCount(fileStat, "Base", "f3()"));
	CHECK(0 == getCallerCount(fileStat, "Base", "f4()"));
	CHECK(0 == getCallerCount(fileStat, "Derived", "f1()"));
	CHECK(1 == getCallerCount(fileStat, "Derived", "f2()"));
	CHECK(0 == getCallerCount(fileStat, "Derived", "f3()"));
	CHECK(1 == getCallerCount(fileStat, "Derived", "f4()"));

	CHECK(0 == getCallerCount(fileStat, "UseOperator(Operator &)"));
	CHECK(1 == getCallerCount(fileStat, "Operator", "operator int()"));
	CHECK(1 == getCallerCount(fileStat, "Operator", "operator*()"));

	CHECK(1 == getCallerCount(fileStat, "call(void (*)())"));
	CHECK(1 == getCallerCount(fileStat, "someFunc()"));
	CHECK(0 == getCallerCount(fileStat, "callSomeFunc()"));

	// implicit destructors call doesn't count
	//CHECK(1 == getCallerCount(fileStat, "Implicit", "Implicit()"));
	//CHECK(1 == getCallerCount(fileStat, "Implicit", "Implicit(const Implicit &)"));
	//CHECK(1 == getCallerCount(fileStat, "Implicit", "~Implicit()"));
	//CHECK(1 == getCallerCount(fileStat, "DefaultCount", "DefaultCount()"));
	//CHECK(1 == getCallerCount(fileStat, "DefaultCount", "DefaultCount(const DefaultCount &)"));
	//CHECK(1 == getCallerCount(fileStat, "DefaultCount", "~DefaultCount()"));
	CHECK(1 == getCallerCount(fileStat, "UserProvided", "UserProvided()"));
	CHECK(1 == getCallerCount(fileStat, "UserProvided", "UserProvided(const UserProvided &)"));
	//CHECK(1 == getCallerCount(fileStat, "UserProvided", "~UserProvided()"));

	CHECK(1 == getCallerCount(fileStat, "New", "New()"));
	//CHECK(1 == getCallerCount(fileStat, "New", "~New()"));

	CHECK(4 == getCallerCount(fileStat, "parameter()"));

	CHECK(1 == getCallerCount(fileStat, "initialize_global()"));

	CHECK(0 == getCallerCount(fileStat, "main()"));
}
