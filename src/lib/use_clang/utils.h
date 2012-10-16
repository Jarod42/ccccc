#ifndef UTILS_H
#define UTILS_H


#define ARRAY_SIZE(a) (sizeof (a) / sizeof(*a))

#include <clang-c/Index.h>
#include <string>

void getStartEndLine(CXSourceRange range, unsigned* startLine, unsigned* endLine);

std::string getStringAndDispose(CXString cxStr);

bool isInFile(const char* filename, CXCursor cursor);

bool isValid(CXTranslationUnit tu);

#endif // UTILS_H
