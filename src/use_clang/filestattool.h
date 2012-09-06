
#ifndef FILESTATTOOL_H
#define FILESTATTOOL_H

#include "../filestat.h"
#include <clang-c/Index.h>

class FileStatTool
{
public:
	static void Compute(const CXTranslationUnit& tu, FileStat* stat);
};

#endif // FILESTATTOOL_H
