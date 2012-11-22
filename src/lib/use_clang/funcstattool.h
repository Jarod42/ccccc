
#ifndef FUNCSTATTOOL_H
#define FUNCSTATTOOL_H

#include "../funcstat.h"
#include <clang-c/Index.h>

namespace ccccc
{
namespace use_clang
{

class FuncStatTool
{
public:
	static void Compute(const CXTranslationUnit& tu, const CXCursor& cursor, FuncStat* stat);
};

} // namespace use_clang
} // namespace ccccc

#endif // FUNCSTATTOOL_H
