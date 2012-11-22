


#ifndef FUNC_STAT_H
#define FUNC_STAT_H

#include "localstat.h"
#include <string>

class FuncStat
{
	friend class FuncStatTool;
public:
	FuncStat(const std::string& funcname, unsigned int line);

	const std::string& getName() const { return m_name; }
	const LocalStat& getStat() const { return m_stat; }
	int getLineDefinition() const { return m_line; }

private:
	std::string m_name;
	LocalStat m_stat;
	int m_line;
};


#endif // FUNC_STAT_H
