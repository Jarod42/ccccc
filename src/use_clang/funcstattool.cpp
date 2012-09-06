

#include "funcstattool.h"

#include "localstattool.h"

void FuncStatTool::Compute(const CXTranslationUnit& tu, const CXCursor& cursor, FuncStat* stat)
{
	LocalStatTool::Compute(tu, cursor, &stat->m_stat);
}
