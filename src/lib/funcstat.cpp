

#include "funcstat.h"

namespace ccccc
{

FuncStat::FuncStat(const std::string& funcname, unsigned int line) :
	m_name(funcname),
	m_line(line)
{
}

} // namespace ccccc
