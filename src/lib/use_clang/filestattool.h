
#ifndef FILESTATTOOL_H
#define FILESTATTOOL_H

#include "../filestat.h"
#include <clang-c/Index.h>

namespace ccccc
{
namespace use_clang
{


class FileStatTool
{
public:
	static void Compute(const CXTranslationUnit& tu, FileStat* stat);
private:
	static enum CXChildVisitResult FileCursorVisitor(CXCursor cursor, CXCursor parent, CXClientData user_data);
	static void VisitNamespace(CXCursor cursor, CXClientData user_data);
};

} // namespace use_clang
} // namespace ccccc

#endif // FILESTATTOOL_H
