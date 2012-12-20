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

#include "utils.h"

namespace ccccc
{
namespace use_clang
{

std::string getStringAndDispose(CXString cxStr)
{
	std::string str = clang_getCString(cxStr);

	clang_disposeString(cxStr);
	return str;
}

unsigned int getStartLine(CXSourceRange range)
{
	CXSourceLocation start = clang_getRangeStart(range);
	unsigned int startLine;
	clang_getExpansionLocation(start, NULL, &startLine, NULL, NULL);

	return startLine;
}

void getStartEndLine(CXSourceRange range, unsigned* startLine, unsigned* endLine)
{
	CXSourceLocation start = clang_getRangeStart(range);
	CXSourceLocation end = clang_getRangeEnd(range);

	clang_getExpansionLocation(start, NULL, startLine, NULL, NULL);
	clang_getExpansionLocation(end, NULL, endLine, NULL, NULL);
}

void getStartEndOffset(CXSourceRange range, unsigned* startoffset, unsigned* endOffset)
{
	CXSourceLocation start = clang_getRangeStart(range);
	CXSourceLocation end = clang_getRangeEnd(range);

	clang_getExpansionLocation(start, NULL, NULL, NULL, startoffset);
	clang_getExpansionLocation(end, NULL, NULL, NULL, endOffset);
}


bool isInFile(const char* filename, CXCursor cursor)
{
	CXSourceRange range = clang_getCursorExtent(cursor);
	if (clang_Range_isNull(range)) {
		return false;
	}
	CXSourceLocation start = clang_getRangeStart(range);
	CXFile file;
	clang_getExpansionLocation(start, &file, NULL, NULL, NULL);
	CXString cxCursorFilename = clang_getFileName(file);
	const char* cstr = clang_getCString(cxCursorFilename);
	if (cstr == NULL) {
		clang_disposeString(cxCursorFilename);
		return false;
	}
	clang_disposeString(cxCursorFilename);
	std::string cursorFilename = cstr;

	return cursorFilename.compare(filename) == 0;
}

bool isValid(CXTranslationUnit tu)
{
	CXCursor cursor = clang_getTranslationUnitCursor(tu);
	unsigned cursorStartLine, cursorEndLine;
	getStartEndLine(clang_getCursorExtent(cursor), &cursorStartLine, &cursorEndLine);

	return cursorStartLine != 0 && cursorStartLine <= cursorEndLine;
}

} // namespace use_clang
} // namespace ccccc
