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

#include "localstat.h"

namespace ccccc
{

LocalStat::LocalStat() :
	lineOfCode_physic(0),
	lineOfCode_program(0),
	lineOfCode_blank(0),
	lineOfCode_comment(0),
	mcCabeCyclomaticNumber(1)
{}

LocalStat::LocalStat(unsigned int lineOfCode_physic,
					 unsigned int lineOfCode_program,
					 unsigned int lineOfCode_blank,
					 unsigned int lineOfCode_comment,
					 unsigned int mcCabeCyclomaticNumber) :
	lineOfCode_physic(lineOfCode_physic),
	lineOfCode_program(lineOfCode_program),
	lineOfCode_blank(lineOfCode_blank),
	lineOfCode_comment(lineOfCode_comment),
	mcCabeCyclomaticNumber(mcCabeCyclomaticNumber)
{}

} // namespace ccccc
