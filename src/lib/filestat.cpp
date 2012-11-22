
#include "filestat.h"

namespace ccccc
{

FileStat::FileStat(const std::string& filename) :
	m_filename(filename),
	m_root("", NULL)
{
}

FileStat::~FileStat()
{
}

FuncStat* FileStat::AddFuncStat(const std::vector<std::string>& namespaceNames, const std::vector<std::string>& classeNames, const std::string& funcname, unsigned int line)
{
	NamespaceStat* namespaceStat = &m_root;
	for (size_t i = 0; i != namespaceNames.size(); ++i) {
		namespaceStat = &namespaceStat->GetOrCreateNamespace(namespaceNames[i]);
	}
	return namespaceStat->AddFuncStat(classeNames, funcname, line);
}

} // namespace ccccc
