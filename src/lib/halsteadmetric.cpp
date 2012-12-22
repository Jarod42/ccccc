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

#include "halsteadmetric.h"

#include <math.h>

namespace ccccc
{

HalsteadMetric::HalsteadMetric() :
	m_vocabularySize(0),
	m_programLength(0),
	m_volume(0),
	m_difficulty(0),
	m_effort(0),
	m_timeToImplement(0),
	m_deliveredBugCount(0)
{
}

HalsteadMetric::HalsteadMetric(unsigned int operatorCount, unsigned int uniqueOperatorCount,
							   unsigned int operandCount, unsigned int uniqueOperandCount)
{
	set(operatorCount, uniqueOperatorCount, operandCount, uniqueOperandCount);
}

void HalsteadMetric::set(unsigned int operatorCount, unsigned int uniqueOperatorCount,
						 unsigned int operandCount, unsigned int uniqueOperandCount)
{
	const unsigned int n1 = uniqueOperatorCount;
	const unsigned int n2 = uniqueOperandCount;
	const unsigned int N1 = operatorCount;
	const unsigned int N2 = operandCount;
	const unsigned int N = N1 + N2;
	const unsigned int n = n1 + n2;
	const double V = N * log2(n);
	const double D = (n1 * N2) / double(2 * n2); // n1 / 2. * double(N2) / n2
	const double E = D * V;
	const double T = E / 18;
	const double B = pow(E, 2. / 3) / 3000;

	m_vocabularySize = n;
	m_programLength = N;
	m_volume = V;
	m_difficulty = D;
	m_effort = E;
	m_timeToImplement = T;
	m_deliveredBugCount = B;
}

} // namespace ccccc
