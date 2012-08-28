#ifndef UTILS_H
#define UTILS_H


#define ARRAY_SIZE(a) (sizeof (a) / sizeof(*a))

#include <clang-c/Index.h>

void getStartEndLine(CXSourceRange range, unsigned* startLine, unsigned* endLine);


#endif // UTILS_H
