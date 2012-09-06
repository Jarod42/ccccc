
#ifndef FUNCSTATTOOL_H
#define FUNCSTATTOOL_H

#include "../funcstat.h"
#include <clang-c/Index.h>

class FuncStatTool
{
public:
	static void Compute(const CXTranslationUnit& tu, const CXCursor& cursor, FuncStat* stat);
};

#endif // FUNCSTATTOOL_H
