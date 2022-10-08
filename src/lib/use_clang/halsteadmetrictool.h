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

#ifndef HALSTEAD_METRIC_TOOL_H
#define HALSTEAD_METRIC_TOOL_H

#include <clang-c/Index.h>

#include <set>
#include <string>

namespace ccccc
{
class HalsteadMetric;
} // namespace ccccc

namespace ccccc::use_clang
{

class HalsteadMetricTool
{
public:
	HalsteadMetricTool() = default;

	void operator()(const CXTranslationUnit& tu, const CXCursor& cursor, const CXToken& token);

	void update(HalsteadMetric* halsteadMetric);

private:
	void AddOperand(const std::string& spelling);
	void AddOperator(const std::string& spelling);

private:
	std::set<std::string> m_operatorSet;
	std::set<std::string> m_operandSet;

	unsigned int m_operatorCount = 0;
	unsigned int m_operandCount = 0;
	unsigned int m_uniqueOperatorCount = 0;
	unsigned int m_uniqueOperandCount = 0;
};

} // namespace ccccc::use_clang

#endif // HALSTEAD_METRIC_TOOL_H
