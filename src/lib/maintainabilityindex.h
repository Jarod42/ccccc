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

#ifndef MAINTAINABILITY_INDEX_H
#define MAINTAINABILITY_INDEX_H

namespace ccccc
{
class HalsteadMetric;
class LineCount;

class MaintainabilityIndex
{
public:
	double getMaintainabilityIndexWithoutComments() const
	{
		return m_maintainabilityIndexWithoutComments;
	}
	double getMaintainabilityIndexCommentWeight() const
	{
		return m_maintainabilityIndexCommentWeight;
	}
	double getMaintainabilityIndex() const { return m_maintainabilityIndex; }

	MaintainabilityIndex() = default;
	MaintainabilityIndex(const LineCount& lineCount,
	                     unsigned int mvg,
	                     const HalsteadMetric& halsteadMetric);

	void set(const LineCount& lineCount, unsigned int mvg, const HalsteadMetric& halsteadMetric);

private:
	double m_maintainabilityIndexWithoutComments = 0.; // MIwoc
	double m_maintainabilityIndexCommentWeight = 0.; // MIcw
	double m_maintainabilityIndex = 0.; // MI
};

} // namespace ccccc

#endif // MAINTAINABILITY_INDEX_H
