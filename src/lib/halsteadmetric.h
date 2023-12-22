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

#ifndef HALSTEAD_METRIC_H
#define HALSTEAD_METRIC_H

#include <cstddef>

namespace ccccc
{

class HalsteadMetric
{
public:
	HalsteadMetric() = default;
	HalsteadMetric(std::size_t operatorCount,
	               std::size_t uniqueOperatorCount,
	               std::size_t operandCount,
	               std::size_t uniqueOperandCount);

	void set(std::size_t operatorCount,
	         std::size_t uniqueOperatorCount,
	         std::size_t operandCount,
	         std::size_t uniqueOperandCount);

	std::size_t getVocabularySize() const { return m_vocabularySize; }
	std::size_t getProgramLength() const { return m_programLength; }
	double getVolume() const { return m_volume; }
	double getDifficulty() const { return m_difficulty; }
	double getEffort() const { return m_effort; }
	double getTimeToImplement() const { return m_timeToImplement; }
	double getDeliveredBugCount() const { return m_deliveredBugCount; }

private:
	std::size_t m_vocabularySize = 0;
	std::size_t m_programLength = 0;
	double m_volume = 0.;
	double m_difficulty = 0.;
	double m_effort = 0.;
	double m_timeToImplement = 0.;
	double m_deliveredBugCount = 0.;
};

} // namespace ccccc

#endif // HALSTEAD_METRIC_H
