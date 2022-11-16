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

void displayFuncNamesList(const ccccc::ClassStat& classStat)
{
	std::cerr << "Methods of " << classStat.getName() << " are:" << std::endl;
	for (std::size_t i = 0; i != classStat.getMethodCount(); ++i) {
		auto& funcStat = classStat.getMethodStat(i);

		std::cerr << funcStat.getName() << std::endl;
	}
}

unsigned int getCallCount(const ccccc::FileStat& fileStat, const char* funcName)
{
	const ccccc::FuncStat* funcStat = fileStat.getFuncStatByName(funcName);

	if (funcStat == nullptr) {
		displayFuncNamesList(fileStat);
		return static_cast<unsigned>(-1);
	}
	return funcStat->getCallCount();
}

unsigned int
getCallCount(const ccccc::FileStat& fileStat, const char* className, const char* funcName)
{
	auto* classStat = fileStat.getClassByName(className);

	if (classStat == nullptr) {
		return static_cast<unsigned>(-1);
	}
	const ccccc::FuncStat* funcStat = classStat->getMethodStatByName(funcName);
	if (funcStat == nullptr) {
		displayFuncNamesList(*classStat);
		return static_cast<unsigned>(-1);
	}
	return funcStat->getCallCount();
}
} // namespace

TEST_CASE("CALL_COUNT_FILE")
{
	ccccc::AllStat stat;
	ccccc::Parameters param;
	const std::string filename = "../../../samples/callcount.cpp";

	param.AddExtra("-std=c++17");
	param.AddFile(filename);
	stat.Compute(param);

	const unsigned int expected = 1;
	CHECK(expected == stat.getFileCount());
	const ccccc::FileStat& fileStat = stat.getFileStat(0);

	CHECK(0 == getCallCount(fileStat, "ADL(void *)"));
	CHECK(0 == getCallCount(fileStat, "ADL(S *)"));
	CHECK(3 == getCallCount(fileStat, "f<>(S &)"));
	CHECK(3 == getCallCount(fileStat, "f(T &)"));
	CHECK(1 == getCallCount(fileStat, "callADL(S &)"));

	CHECK(2 == getCallCount(fileStat, "UseOperator(Operator &)"));
	CHECK(0 == getCallCount(fileStat, "Operator", "operator int()"));
	CHECK(0 == getCallCount(fileStat, "Operator", "operator*()"));

	CHECK(1 == getCallCount(fileStat, "call(void (*)())"));
	CHECK(0 == getCallCount(fileStat, "someFunc()"));
	CHECK(1 == getCallCount(fileStat, "callSomeFunc()"));

	// implicit destructors call doesn't count
	CHECK(2 == getCallCount(fileStat, "ImplicitCount()"));
	CHECK(2 == getCallCount(fileStat, "DefaultCount()"));
	CHECK(2 == getCallCount(fileStat, "UserProvidedCount()"));

	CHECK(2 == getCallCount(fileStat, "newDelete()"));

	CHECK(5 == getCallCount(fileStat, "function_as_parameter()"));

	CHECK(0 == getCallCount(fileStat, "main()"));
}
