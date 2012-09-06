

#include "utils.h"

std::string getStringAndDispose(CXString cxStr)
{
	std::string str = clang_getCString(cxStr);

	clang_disposeString(cxStr);
	return str;
}

void getStartEndLine(CXSourceRange range, unsigned* startLine, unsigned* endLine)
{
	CXSourceLocation start = clang_getRangeStart(range);
	CXSourceLocation end = clang_getRangeEnd(range);

	clang_getExpansionLocation(start, NULL, startLine, NULL, NULL);
	clang_getExpansionLocation(end, NULL, endLine, NULL, NULL);
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