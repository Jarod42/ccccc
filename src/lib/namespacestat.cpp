

#include "namespacestat.h"
#include "classstat.h"
#include "funcstat.h"

namespace ccccc
{

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

const FuncStat* NamespaceStat::getFuncStatByName(const char* funcNameId) const
{
	for (size_t i = 0; i != m_funcStats.size(); ++i) {
		if (m_funcStats[i]->getName().compare(funcNameId) == 0) {
			return m_funcStats[i];
		}
	}
	return NULL;
}

const NamespaceStat* NamespaceStat::getNamespaceByName(const char* namespaceName) const
{
	NamespaceStatConstIterator it = m_namespaces.find(namespaceName);

	if (it != m_namespaces.end()) {
		return (*it).second;
	}
	return NULL;
}

const ClassStat* NamespaceStat::getClassByName(const char* ClassName) const
{
	ClassStatConstIterator it = m_classes.find(ClassName);

	if (it != m_classes.end()) {
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

ClassStat& NamespaceStat::GetOrCreateClass(const std::string& className)
{
	ClassStatConstIterator it = m_classes.find(className);

	if (it != m_classes.end()) {
		return *(*it).second;
	}
	ClassStat* classStat = new ClassStat(className, NULL, this);
	m_classes.insert(make_pair(className, classStat));
	return *classStat;
}

FuncStat* NamespaceStat::AddFuncStat(const std::vector<std::string>& classeNames, const std::string& funcname, unsigned int line)
{
	if (classeNames.empty()) {
		FuncStat* stat = new FuncStat(funcname, line);

		m_funcStats.push_back(stat);
		return stat;
	}
	ClassStat* classStat = &GetOrCreateClass(classeNames[0]);
	for (size_t i = 1; i != classeNames.size(); ++i) {
		classStat = &classStat->GetOrCreateClass(classeNames[i]);
	}
	return classStat->AddMethodStat(funcname, line);
}

} // namespace ccccc
