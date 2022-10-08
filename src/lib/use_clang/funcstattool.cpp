/*
** Copyright 2012-2022 Joris Dauphin
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
#include "halsteadmetrictool.h"
#include "linecounter.h"
#include "localstattool.h"
#include "mccabecyclomaticnumber.h"

namespace ccccc::use_clang
{

namespace
{

using CallerUserData = std::pair<int, CallerCountData*>;

class FuncStatFeeder
{
public:
	explicit FuncStatFeeder(const CXCursor& cursor) :
		m_lineCounter(cursor)
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
	auto* callerUserData = reinterpret_cast<CallerUserData*>(user_data);
	auto& callCount = callerUserData->first;
	auto* callerCountData = callerUserData->second;
	auto cursorKind = clang_getCursorKind(cursor);
	if (cursorKind == CXCursorKind::CXCursor_DeclRefExpr
	    || cursorKind == CXCursorKind::CXCursor_MemberRefExpr) {
		const auto referencedCursor = clang_getCursorReferenced(cursor);
		auto refcursorKind = clang_getCursorKind(referencedCursor);

		if (refcursorKind == CXCursorKind::CXCursor_CXXMethod
		    || refcursorKind == CXCursorKind::CXCursor_FunctionDecl) {
			if (callerCountData->m_templatedVisited.count(cursor) != 0) {
				return CXChildVisitResult::CXChildVisit_Recurse;
			}
			callerCountData->m_templatedVisited.insert(cursor);
			const auto usr = getStringAndDispose(clang_getCursorUSR(referencedCursor));
			callerCountData->m_counts[referencedCursor]++;
			callerCountData->m_usrCounts[usr]++;
			callCount++;

			if (clang_Cursor_getNumTemplateArguments(referencedCursor) != -1) {
				// non specialized template instanciation
				// counts for the generic template
				if (callerCountData->m_templatedSpecializations.count(referencedCursor) == 0) {
					auto templateCursor = clang_getSpecializedCursorTemplate(referencedCursor);
					const auto templateusr = getStringAndDispose(clang_getCursorUSR(templateCursor));
					callerCountData->m_counts[templateCursor]++;
					callerCountData->m_usrCounts[templateusr]++;
				}
				// For template, follow the instantiation
				// to be able to call dependent code.
				CallerUserData userData(0, callerCountData);
				clang_visitChildren(referencedCursor, &CallerCounterVisitor, &userData);
			}
		}
	}
	return CXChildVisitResult::CXChildVisit_Recurse;
}

bool IsOverridenMethod(const CXCursor& cursor)
{
	CXCursor* overriden = nullptr;
	unsigned int count;
	clang_getOverriddenCursors(cursor, &overriden, &count);
	clang_disposeOverriddenCursors(overriden);
	return count != 0;
}

template <typename Map>
auto Value(const Map& m,
           const typename Map::key_type& key,
           const typename Map::mapped_type& default_value)
{
	const auto it = m.find(key);
	return it == m.end() ? default_value : it->second;
}

} // anonymous namespace

void FuncStatTool::Compute(const char* filename, const CXTranslationUnit& tu, const CXCursor& cursor, GlobalData& globalData, FuncStat* stat)
{
	stat->m_usr = getStringAndDispose(clang_getCursorUSR(cursor));
	stat->m_cursor = cursor;

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

	stat->m_isConst = clang_CXXMethod_isConst(cursor);
	stat->m_isStatic = clang_CXXMethod_isStatic(cursor);
	stat->m_isVirtual = clang_CXXMethod_isVirtual(cursor);
	stat->m_isOverriden = IsOverridenMethod(cursor);
	if (clang_Cursor_getNumTemplateArguments(cursor) != -1) {
		globalData.m_callerCountData.m_templatedSpecializations.insert(cursor);
	}
	CallerUserData userData(0, &globalData.m_callerCountData);
	clang_visitChildren(cursor, &CallerCounterVisitor, &userData);
	stat->m_callCount = userData.first;
}

void FuncStatTool::PostFeed(const GlobalData& globalData, FuncStat* stat)
{
	const auto& data = globalData.m_callerCountData;
	const std::size_t cursorCount = Value(data.m_counts, stat->m_cursor, 0u);
	const std::size_t usrCount = Value(data.m_usrCounts, stat->m_usr, 0u);

	stat->m_callerCount = std::max(usrCount, cursorCount);
}

} // namespace ccccc::use_clang
