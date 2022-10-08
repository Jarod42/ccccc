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

#include "halsteadmetrictool.h"

#include "../halsteadmetric.h"
#include "utils.h"

#include <clang-c/Index.h>

#include <math.h>

namespace ccccc::use_clang
{

void HalsteadMetricTool::update(HalsteadMetric* halsteadMetric)
{
	halsteadMetric->set(m_operatorCount, m_uniqueOperatorCount, m_operandCount, m_uniqueOperandCount);
}

void HalsteadMetricTool::AddOperand(const std::string& spelling)
{
	++m_operandCount;
	if (m_operandSet.insert(spelling).second) {
		++m_uniqueOperandCount;
	}
}

void HalsteadMetricTool::AddOperator(const std::string& spelling)
{
	++m_operatorCount;
	if (m_operatorSet.insert(spelling).second) {
		++m_uniqueOperatorCount;
	}
}

void HalsteadMetricTool::operator()(const CXTranslationUnit& tu, const CXCursor& /*cursor*/, const CXToken& token)
{
	if (clang_getTokenKind(token) == CXToken_Comment) {
		return;
	}
	const std::string spelling = getStringAndDispose(clang_getTokenSpelling(tu, token));
	if (clang_getTokenKind(token) == CXToken_Punctuation) {
		AddOperator(spelling);
	} else if (clang_getTokenKind(token) == CXToken_Literal) {
		AddOperand(spelling);
	} else if (clang_getTokenKind(token) == CXToken_Keyword) {
		CXCursor cursor = getCursor(tu, token);

		if (clang_isDeclaration(clang_getCursorKind(cursor)) == false) {
			AddOperator(spelling);
		} else {
			AddOperand(spelling);
		}
	} else if (clang_getTokenKind(token) == CXToken_Identifier) {
		AddOperand(spelling);
	}
}

} // namespace ccccc::use_clang
