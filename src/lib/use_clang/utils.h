#ifndef UTILS_H
#define UTILS_H


#define ARRAY_SIZE(a) (sizeof (a) / sizeof(*a))

#include <clang-c/Index.h>
#include <string>

namespace ccccc
{
namespace use_clang
{


unsigned int getStartLine(CXSourceRange range);
void getStartEndLine(CXSourceRange range, unsigned* startLine, unsigned* endLine);

std::string getStringAndDispose(CXString cxStr);

bool isInFile(const char* filename, CXCursor cursor);

bool isValid(CXTranslationUnit tu);

} // namespace use_clang
} // namespace ccccc

#endif // UTILS_H
