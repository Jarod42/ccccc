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

#ifndef LINECOUNTER_H
#define LINECOUNTER_H

#include <clang-c/Index.h>
#include <cstddef>

namespace ccccc::use_clang
{

class LineCounter
{
public:
	explicit LineCounter(const CXCursor& cursor);

	void operator()(const CXTranslationUnit& tu, const CXCursor& cursor, const CXToken& token);

	std::size_t getLineOfCode_physic() const { return lineOfCode_physic; }
	std::size_t getLineOfCode_comment() const { return lineOfCode[0]; }
	std::size_t getLineOfCode_program() const { return lineOfCode[1]; }
	std::size_t getLineOfCode_blank() const { return lineOfCode[2]; }

private:
	std::size_t lineOfCode_physic;
	// com, loc, blankLine
	std::size_t lastLine[2];
	std::size_t lineOfCode[3];
};

} // namespace ccccc::use_clang

#endif // LINECOUNTER_H
