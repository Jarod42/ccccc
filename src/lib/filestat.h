


#ifndef FILE_STAT_H
#define FILE_STAT_H

#include "localstat.h"
#include "funcstat.h"

#include <string>
#include <vector>

class FileStat
{
	friend class FileStatTool;
public:
	explicit FileStat(const std::string& filename);
	~FileStat();

	const std::string& getFilename() const { return m_filename; }
	const LocalStat& getStat() const { return m_stat; }
	unsigned int getFunctionCount() const { return m_funcStats.size(); }
	const FuncStat& getFuncStat(unsigned int index) const { return *m_funcStats[index]; }
	const FuncStat* getFuncStatByName(const char *funcNameId) const;

private:
	FuncStat* AddFuncStat(const std::string& funcname);
private:
	std::string m_filename;
	LocalStat m_stat;
	std::vector<FuncStat*> m_funcStats;
};


#endif // FILE_STAT_H
