
#ifndef FILESTATTOOL_H
#define FILESTATTOOL_H

#include "../filestat.h"
#include <clang-c/Index.h>

class FileStatTool
{
public:
	static void Compute(const CXTranslationUnit& tu, FileStat* stat);
private:
	static enum CXChildVisitResult FileCursorVisitor(CXCursor cursor, CXCursor parent, CXClientData user_data);
	static void VisitNamespace(CXCursor cursor, CXClientData user_data);
};

#endif // FILESTATTOOL_H
