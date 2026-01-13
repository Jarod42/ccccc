/*
** Copyright 2012-2026 Joris Dauphin
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

#ifndef FUNC_STAT_H
#define FUNC_STAT_H

#include "halsteadmetric.h"
#include "linecount.h"
#include "maintainabilityindex.h"

#include <clang-c/Index.h>

#include <string>

namespace ccccc
{
namespace use_clang
{
class FuncStatTool;
} // namespace use_clang

class FuncStat
{
	friend class use_clang::FuncStatTool;

public:
	FuncStat(const std::string& funcname, std::size_t line);

	const std::string& getName() const { return m_name; }
	bool isConst() const { return m_isConst; }
	bool isStatic() const { return m_isStatic; }
	bool isVirtual() const { return m_isVirtual; }
	bool isOverridden() const { return m_isOverridden; }
	bool isExplicit() const { return m_isExplicit; }
	const LineCount& getLineCount() const { return m_lineCount; }
	std::size_t getParameterCount() const { return m_parameterCount; }
	std::size_t getMcCabeCyclomaticNumber() const { return m_mcCabeCyclomaticNumber; }
	std::size_t getLineDefinition() const { return m_line; }
	const HalsteadMetric& getHalsteadMetric() const { return m_halsteadMetric; }
	const MaintainabilityIndex& getMaintainabilityIndex() const { return m_maintainabilityIndex; }
	std::size_t getNestedBlockCount() const { return m_nestedBlockCount; }
	std::size_t getCallCount() const { return m_callCount; }
	std::size_t getCallerCount() const { return m_callerCount; }

private:
	std::string m_name;
	bool m_isConst = false;
	bool m_isStatic = false;
	bool m_isVirtual = false;
	bool m_isOverridden = false;
	bool m_isExplicit = false;
	LineCount m_lineCount;
	std::size_t m_parameterCount = 0;
	std::size_t m_line = 0;
	std::size_t m_mcCabeCyclomaticNumber = 0; // MVG
	HalsteadMetric m_halsteadMetric;
	MaintainabilityIndex m_maintainabilityIndex;
	std::size_t m_nestedBlockCount = 0;
	//  A Unified Symbol Resolution (USR) is a string
	// that identifies a particular entity
	std::string m_usr;
	CXCursor m_cursor; // Identifier
	std::size_t m_callCount = 0; // How many function this function call
	std::size_t m_callerCount = 0; // How many time this function is called
};

} // namespace ccccc

#endif // FUNC_STAT_H
