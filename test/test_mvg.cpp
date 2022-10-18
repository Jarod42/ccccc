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

#include <UnitTest++/UnitTest++.h>

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
