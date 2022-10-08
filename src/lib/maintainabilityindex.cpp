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

#include "maintainabilityindex.h"

#include "halsteadmetric.h"
#include "linecount.h"

#include <math.h>

namespace ccccc
{

MaintainabilityIndex::MaintainabilityIndex(const LineCount& lineCount,
                                           unsigned int mvg,
                                           const HalsteadMetric& halsteadMetric)
{
	set(lineCount, mvg, halsteadMetric);
}

void MaintainabilityIndex::set(const LineCount& lineCount,
                               unsigned int mvg,
                               const HalsteadMetric& halsteadMetric)
{
	const double MIwoc = 171 - 5.2 * log(halsteadMetric.getVolume()) - 0.23 * mvg
	                   - 16.2 * log(lineCount.getLineOfCode_physic());
	const double perCM =
		double(lineCount.getLineOfCode_comment()) / lineCount.getLineOfCode_physic();
	const double MIcw = 50 * sin(sqrt(2.4 * perCM));

	m_maintainabilityIndexWithoutComments = MIwoc;
	m_maintainabilityIndexCommentWeight = MIcw;
	m_maintainabilityIndex = MIwoc + MIcw;
}

} // namespace ccccc
