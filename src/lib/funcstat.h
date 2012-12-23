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

#ifndef FUNC_STAT_H
#define FUNC_STAT_H

#include "halsteadmetric.h"
#include "linecount.h"
#include "maintainabilityindex.h"
#include <string>

namespace ccccc
{
namespace use_clang
{
class FuncStatTool;
}

class FuncStat
{
	friend class use_clang::FuncStatTool;
public:
	FuncStat(const std::string& funcname, unsigned int line);

	const std::string& getName() const { return m_name; }
	const LineCount& getLineCount() const { return m_lineCount; }
	unsigned int getMcCabeCyclomaticNumber() const { return m_mcCabeCyclomaticNumber; }
	int getLineDefinition() const { return m_line; }
	const HalsteadMetric& getHalsteadMetric() const { return m_halsteadMetric; }
	const MaintainabilityIndex& getMaintainabilityIndex() const { return m_maintainabilityIndex; }
private:
	std::string m_name;
	LineCount m_lineCount;
	int m_line;
	unsigned int m_mcCabeCyclomaticNumber; // MVG
	HalsteadMetric m_halsteadMetric;
	MaintainabilityIndex m_maintainabilityIndex;
};

} // namespace ccccc

#endif // FUNC_STAT_H
