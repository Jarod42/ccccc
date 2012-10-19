

#include "classstat.h"
#include "funcstat.h"

ClassStat::ClassStat(const std::string& name, NamespaceStat* parent) :
	m_name(name),
	m_parent(parent)
{
}

ClassStat::~ClassStat()
{
	for (size_t i = 0; i != m_funcStats.size(); ++i) {
		delete m_funcStats[i];
	}
	for (NamespaceStatConstIterator it = m_namespaces.begin(); it != m_namespaces.end(); ++it) {
		delete it->second;
	}
}

const FuncStat* ClassStat::getFuncStatByName(const char *funcNameId) const
{
	for (size_t i = 0; i != m_funcStats.size(); ++i) {
		if (m_funcStats[i]->getName().compare(funcNameId) == 0) {
			return m_funcStats[i];
		}
	}
	return NULL;
}

const ClassStat* ClassStat::getClassByName(const char *className) const
{
	NamespaceStatConstIterator it = m_namespaces.find(namespaceName);

	if (it != m_namespaces.end()) {
		return (*it).second;
	}
	return NULL;
}

ClassStat& ClassStat::GetOrCreateClass(const std::string& className)
{
	NamespaceStatConstIterator it = m_namespaces.find(namespaceName);

	if (it != m_namespaces.end()) {
		return *(*it).second;
	}
	ClassStat* namespaceStat = new ClassStat(namespaceName, this);
	m_namespaces.insert(make_pair(namespaceName, namespaceStat));
	return *namespaceStat;
}

FuncStat* ClassStat::AddMethodStat(const std::string& methodName)
{
	FuncStat* stat = new FuncStat(methodName);

	m_funcStats.push_back(stat);
	return stat;
}
