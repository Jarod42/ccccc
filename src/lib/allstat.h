/*
** Copyright 2012-2014 Joris Dauphin
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

#ifndef ALL_STAT_H
#define ALL_STAT_H

#include <memory>
#include <vector>

namespace ccccc
{

class FileStat;
class Parameters;

class AllStat
{
public:
	~AllStat();

	void Compute(const Parameters& param);

	unsigned int getFileCount() const { return m_filesStat.size(); }
	const FileStat& getFileStat(unsigned int index) const { return *m_filesStat[index]; }

private:
	std::vector<std::unique_ptr<FileStat>> m_filesStat;
};

} // namespace ccccc

#endif // ALL_STAT_H
