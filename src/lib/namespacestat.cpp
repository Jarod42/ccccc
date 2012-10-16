

#include "namespacestat.h"
#include "funcstat.h"

NamespaceStat::NamespaceStat(const std::string& name, NamespaceStat* parent) :
	m_name(name),
	m_parent(parent)
{
}

NamespaceStat::~NamespaceStat()
{
	for (size_t i = 0; i != m_funcStats.size(); ++i) {
		delete m_funcStats[i];
	}
	for (NamespaceStatConstIterator it = m_namespaces.begin(); it != m_namespaces.end(); ++it) {
		delete it->second;
	}
}

const FuncStat* NamespaceStat::getFuncStatByName(const char *funcNameId) const
{
	for (size_t i = 0; i != m_funcStats.size(); ++i) {
		if (m_funcStats[i]->getName().compare(funcNameId) == 0) {
			return m_funcStats[i];
		}
	}
	return NULL;
}

const NamespaceStat* NamespaceStat::getNamespaceByName(const char *namespaceName) const
{
	NamespaceStatConstIterator it = m_namespaces.find(namespaceName);

	if (it != m_namespaces.end()) {
		return (*it).second;
	}
	return NULL;
}

NamespaceStat& NamespaceStat::GetOrCreateNamespace(const std::string& namespaceName)
{
	NamespaceStatConstIterator it = m_namespaces.find(namespaceName);

	if (it != m_namespaces.end()) {
		return *(*it).second;
	}
	NamespaceStat* namespaceStat = new NamespaceStat(namespaceName, this);
	m_namespaces.insert(make_pair(namespaceName, namespaceStat));
	return *namespaceStat;
}

FuncStat* NamespaceStat::AddFuncStat(const std::string& funcname)
{
	FuncStat* stat = new FuncStat(funcname);

	m_funcStats.push_back(stat);
	return stat;
}
