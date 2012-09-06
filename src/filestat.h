


#ifndef FILE_STAT_H
#define FILE_STAT_H

#include "localstat.h"
#include <string>

class FileStat
{
	friend class FileStatTool;
public:
	explicit FileStat(const std::string& filename);

	const std::string& getFilename() const { return m_filename; }
	LocalStat getStat() const { return m_stat; }
//	unsigned int getFunctionCount() const;


private:
	std::string m_filename;
	LocalStat m_stat;
};


#endif // FILE_STAT_H
