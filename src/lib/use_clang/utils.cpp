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

#include "utils.h"

namespace ccccc::use_clang
{

std::string getStringAndDispose(CXString cxStr)
{
	const std::string str = clang_getCString(cxStr);

	clang_disposeString(cxStr);
	return str;
}

CXCursor getCursor(const CXTranslationUnit& tu, const CXToken& token)
{
	return clang_getCursor(tu, clang_getTokenLocation(tu, token));
}

unsigned int getStartLine(CXSourceRange range)
{
	const CXSourceLocation start = clang_getRangeStart(range);
	unsigned int startLine;
	clang_getExpansionLocation(start, nullptr, &startLine, nullptr, nullptr);
	return startLine;
}

void getStartEndLine(CXSourceRange range, unsigned* startLine, unsigned* endLine)
{
	const CXSourceLocation start = clang_getRangeStart(range);
	const CXSourceLocation end = clang_getRangeEnd(range);

	clang_getExpansionLocation(start, nullptr, startLine, nullptr, nullptr);
	clang_getExpansionLocation(end, nullptr, endLine, nullptr, nullptr);
}

void getStartEndOffset(CXSourceRange range, unsigned* startoffset, unsigned* endOffset)
{
	const CXSourceLocation start = clang_getRangeStart(range);
	const CXSourceLocation end = clang_getRangeEnd(range);

	clang_getExpansionLocation(start, nullptr, nullptr, nullptr, startoffset);
	clang_getExpansionLocation(end, nullptr, nullptr, nullptr, endOffset);
}

bool isInFile(const std::filesystem::path& filename, CXCursor cursor)
{
	const CXSourceRange range = clang_getCursorExtent(cursor);
	if (clang_Range_isNull(range)) {
		return false;
	}
	const CXSourceLocation start = clang_getRangeStart(range);
	CXFile file;
	clang_getExpansionLocation(start, &file, nullptr, nullptr, nullptr);
	const CXString cxCursorFilename = clang_getFileName(file);
	const char* cstr = clang_getCString(cxCursorFilename);
	if (cstr == nullptr) {
		clang_disposeString(cxCursorFilename);
		return false;
	}
	clang_disposeString(cxCursorFilename);
	const std::string cursorFilename = cstr;
	return cursorFilename.compare(filename.string()) == 0;
}

bool isValid(CXTranslationUnit tu)
{
	const CXCursor cursor = clang_getTranslationUnitCursor(tu);
	unsigned cursorStartLine, cursorEndLine;
	getStartEndLine(clang_getCursorExtent(cursor), &cursorStartLine, &cursorEndLine);

	return cursorStartLine != 0 && cursorStartLine <= cursorEndLine;
}

} // namespace ccccc::use_clang
