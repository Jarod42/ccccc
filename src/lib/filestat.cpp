
#include "filestat.h"


FileStat::FileStat(const std::string& filename) :
	m_filename(filename),
	m_root("", NULL)
{
}

FileStat::~FileStat()
{
}

FuncStat* FileStat::AddFuncStat(const std::string& funcname, const std::vector<std::string>& namespaceNames)
{
	NamespaceStat* namespaceStat = &m_root;
	for (size_t i = 0; i != namespaceNames.size(); ++i) {
		namespaceStat = &namespaceStat->GetOrCreateNamespace(namespaceNames[i]);
	}
	return namespaceStat->AddFuncStat(funcname);
}
