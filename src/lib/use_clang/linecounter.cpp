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

#include "linecounter.h"

#include <clang-c/Index.h>

#include "utils.h"
#include <algorithm>

namespace ccccc
{
namespace use_clang
{

LineCounter::LineCounter(const CXCursor& cursor)
{
	unsigned cursorStartLine, cursorEndLine;
	getStartEndLine(clang_getCursorExtent(cursor), &cursorStartLine, &cursorEndLine);
	lineOfCode_physic = cursorEndLine - cursorStartLine + 1;
	lastLine[0] = lastLine[1] = cursorStartLine - 1;
	lineOfCode[0] = lineOfCode[1] = lineOfCode[2] = 0;
}

void LineCounter::operator () (const CXTranslationUnit& tu, const CXCursor& cursor, const CXToken& token)
{
	unsigned startLine, endLine;
	getStartEndLine(clang_getTokenExtent(tu, token), &startLine, &endLine);
	unsigned int type = (clang_getTokenKind(token) == CXToken_Comment) ? 0 : 1;

	if (startLine != lastLine[type]) {
		lineOfCode[type] += endLine - startLine + 1;
		unsigned int maxLast = std::max(lastLine[0], lastLine[1]);
		if (startLine > maxLast) {
			lineOfCode[2] += startLine - maxLast - 1;
		}
	} else {
		lineOfCode[type] += endLine - startLine;
	}
	lastLine[type] = endLine;
}

} //namespace use_clang
} // namespace ccccc

