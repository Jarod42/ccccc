

#include "filestat.h"


FileStat::FileStat(const std::string& filename) :
	m_filename(filename)
{
}

FileStat::~FileStat()
{
	for (size_t i = 0; i != m_funcStats.size(); ++i) {
		delete m_funcStats[i];
	}
}

FuncStat* FileStat::AddFuncStat(const std::string& funcname)
{
	FuncStat* stat = new FuncStat(funcname);

	m_funcStats.push_back(stat);
	return stat;
}
