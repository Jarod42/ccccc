
#ifndef LOCALSTATTOOL_H
#define LOCALSTATTOOL_H

#include "../localstat.h"
#include <clang-c/Index.h>

class LocalStatTool
{
public:
	static void Compute(const CXTranslationUnit& tu, const CXCursor& cursor, LocalStat* stat);
private:
	static void UpdateMcCabeCyclomaticNumber(const CXTranslationUnit& tu, const CXToken& token, LocalStat* stat);
};

#endif // LOCALSTATTOOL_H
