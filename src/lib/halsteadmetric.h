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

#ifndef HALSTEAD_METRIC_H
#define HALSTEAD_METRIC_H

namespace ccccc
{

class HalsteadMetric
{
public:
	HalsteadMetric();
	HalsteadMetric(unsigned int operatorCount, unsigned int uniqueOperatorCount,
				   unsigned int operandCount, unsigned int uniqueOperandCount);

	void set(unsigned int operatorCount, unsigned int uniqueOperatorCount,
			 unsigned int operandCount, unsigned int uniqueOperandCount);

	unsigned int getVocabularySize() const { return m_vocabularySize; }
	unsigned int getProgramLength() const { return m_programLength; }
	double getVolume() const { return m_volume; }
	double getDifficulty() const { return m_difficulty; }
	double getEffort() const { return m_effort; }
	double getTimeToImplement() const { return m_timeToImplement; }
	double getDeliveredBugCount() const { return m_deliveredBugCount; }

private:
	unsigned int m_vocabularySize;
	unsigned int m_programLength;
	double m_volume;
	double m_difficulty;
	double m_effort;
	double m_timeToImplement;
	double m_deliveredBugCount;
};

} // namespace ccccc

#endif // HALSTEAD_METRIC_H
