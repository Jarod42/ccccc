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

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#define CHECK_EQUAL_LOC(lhs, rhs) \
 CHECK_EQUAL((lhs).getLineOfCode_blank(), (rhs).getLineOfCode_blank()); \
 CHECK_EQUAL((lhs).getLineOfCode_comment(), (rhs).getLineOfCode_comment()); \
 CHECK_EQUAL((lhs).getLineOfCode_physic(), (rhs).getLineOfCode_physic()); \
 CHECK_EQUAL((lhs).getLineOfCode_program(), (rhs).getLineOfCode_program());

#endif
