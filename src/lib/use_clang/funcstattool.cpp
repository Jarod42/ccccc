/*
** Copyright 2012 Joris Dauphin
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

#include "localstattool.h"
#include "linecounter.h"
#include "mccabecyclomaticnumber.h"

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

	void operator () (const CXTranslationUnit& tu, const CXCursor& cursor, const CXToken& token)
	{
		m_mvg(tu, cursor, token);
		m_lineCounter(tu, cursor, token);
	}

public:
	McCabeCyclomaticNumber m_mvg;
	LineCounter m_lineCounter;
};

} // anonymous namespace

void FuncStatTool::Compute(const CXTranslationUnit& tu, const CXCursor& cursor, FuncStat* stat)
{
	FuncStatFeeder funcStatFeeder(cursor);
	processTokens(tu, cursor, funcStatFeeder);

	stat->m_lineCount.lineOfCode_physic = funcStatFeeder.m_lineCounter.getLineOfCode_physic();
	stat->m_lineCount.lineOfCode_comment = funcStatFeeder.m_lineCounter.getLineOfCode_comment();
	stat->m_lineCount.lineOfCode_program = funcStatFeeder.m_lineCounter.getLineOfCode_program();
	stat->m_lineCount.lineOfCode_blank = funcStatFeeder.m_lineCounter.getLineOfCode_blank();
	stat->m_lineCount.mcCabeCyclomaticNumber = funcStatFeeder.m_mvg.getValue();
}

} // namespace use_clang
} // namespace ccccc
