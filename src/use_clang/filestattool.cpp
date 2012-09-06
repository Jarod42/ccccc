

#include "filestattool.h"

#include "localstattool.h"

void FileStatTool::Compute(const CXTranslationUnit& tu, FileStat* stat)
{
	CXCursor cursor = clang_getTranslationUnitCursor(tu);

	LocalStatTool::Compute(tu, cursor, &stat->m_stat);
}
