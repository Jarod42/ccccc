/*
** Copyright 2012-2015 Joris Dauphin
*/
/*
**  This file is part of CCCCC.
**
**  CCCCC is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  CCCCC is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with CCCCC. If not, see <http://www.gnu.org/licenses/>.
*/

#include "funcstattool.h"

#include "../caller_count.h"
#include "../globaldata.h"
#include "blockcounter.h"
#include "localstattool.h"
#include "linecounter.h"
#include "mccabecyclomaticnumber.h"
#include "halsteadmetrictool.h"

namespace ccccc
{
namespace use_clang
{

namespace
{
class FuncStatFeeder
{
public:
	explicit FuncStatFeeder(const CXCursor& cursor) : m_lineCounter(cursor)
	{
	}

	void operator()(const CXTranslationUnit& tu, const CXCursor& cursor, const CXToken& token)
	{
		m_halsteadMetricTool(tu, cursor, token);
		m_mvg(tu, cursor, token);
		m_lineCounter(tu, cursor, token);
	}

public:
	HalsteadMetricTool m_halsteadMetricTool;
	McCabeCyclomaticNumber m_mvg;
	LineCounter m_lineCounter;
};

enum CXChildVisitResult
CallerCounterVisitor(CXCursor cursor,
					 CXCursor /*parent*/,
					 CXClientData user_data)
{
	auto* callerCountData = reinterpret_cast<CallerCountData*>(user_data);
	auto cursorKind = clang_getCursorKind(cursor);
	if (cursorKind == CXCursorKind::CXCursor_CallExpr) {
		const auto referencedCursor = clang_getCursorReferenced(cursor);
		const auto usr = getStringAndDispose(clang_getCursorUSR(referencedCursor));

		callerCountData->m_counts[usr]++;
	}
	return CXChildVisitResult::CXChildVisit_Recurse;
}

} // anonymous namespace

void FuncStatTool::Compute(const char* filename, const CXTranslationUnit& tu, const CXCursor& cursor, GlobalData& globalData, FuncStat* stat)
{
	stat->m_usr = getStringAndDispose(clang_getCursorUSR(cursor));

	FuncStatFeeder funcStatFeeder(cursor);
	processTokens(tu, cursor, funcStatFeeder);

	stat->m_lineCount.lineOfCode_physic = funcStatFeeder.m_lineCounter.getLineOfCode_physic();
	stat->m_lineCount.lineOfCode_comment = funcStatFeeder.m_lineCounter.getLineOfCode_comment();
	stat->m_lineCount.lineOfCode_program = funcStatFeeder.m_lineCounter.getLineOfCode_program();
	stat->m_lineCount.lineOfCode_blank = funcStatFeeder.m_lineCounter.getLineOfCode_blank();
	stat->m_mcCabeCyclomaticNumber = funcStatFeeder.m_mvg.getValue();
	funcStatFeeder.m_halsteadMetricTool.update(&stat->m_halsteadMetric);
	stat->m_maintainabilityIndex.set(stat->m_lineCount, stat->m_mcCabeCyclomaticNumber, stat->getHalsteadMetric());
	stat->m_nestedBlockCount = BlockCounter::ComputeNestedBlockCount(filename, cursor) - 1;

	clang_visitChildren(cursor, &CallerCounterVisitor, &globalData.m_callerCountData);
}

void FuncStatTool::PostFeed(const GlobalData& globalData, FuncStat* stat)
{
	const auto& m = globalData.m_callerCountData.m_counts;

	const auto it = m.find(stat->m_usr);
	stat->m_callerCount = it == m.end() ? 0 : it->second;
}

} // namespace use_clang

} // namespace ccccc
