

#include "funcstattool.h"

#include "localstattool.h"

namespace ccccc
{
namespace use_clang
{

void FuncStatTool::Compute(const CXTranslationUnit& tu, const CXCursor& cursor, FuncStat* stat)
{
	LocalStatTool::Compute(tu, cursor, &stat->m_stat);
}

} // namespace use_clang
} // namespace ccccc
