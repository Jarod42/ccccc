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
#include "parameters.h"

#include <UnitTest++/UnitTest++.h>

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
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_do_while(int)"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_for_noblock(const char *)"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_forfor(const char (&)[5][5])"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_forfor_no_block(const char (&)[5][5])"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_switch(int)"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_switch_case(int)"));
	CHECK_EQUAL(1, CheckBlockCount(fileStat, "function_try()"));
	CHECK_EQUAL(0, CheckBlockCount(fileStat, "function_try_function()"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_block()"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_lambda()"));
	CHECK_EQUAL(2, CheckBlockCount(fileStat, "function_max_block()"));
}
