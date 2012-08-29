

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
