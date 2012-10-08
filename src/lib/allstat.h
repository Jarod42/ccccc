#ifndef ALL_STAT_H
#define ALL_STAT_H

#include <vector>

#include "filestat.h"

namespace ccccc
{

class Parameters;

class AllStat
{
public:
	~AllStat();

	void Compute(const Parameters &param);

	unsigned int getFileCount() const { return m_filesStat.size(); }
	const FileStat& getFileStat(unsigned int index) const { return *m_filesStat[index]; }

private:
	std::vector<FileStat*> m_filesStat;
};

}

#endif // ALL_STAT_H
