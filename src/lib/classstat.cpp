

#include "classstat.h"
#include "funcstat.h"

namespace ccccc
{

ClassStat::ClassStat(const std::string& name, ClassStat* classParent, NamespaceStat* namespaceParent) :
	m_name(name),
	m_namespaceParent(namespaceParent),
	m_classParent(classParent)
{
}

ClassStat::~ClassStat()
{
	for (size_t i = 0; i != m_methodStats.size(); ++i) {
		delete m_methodStats[i];
	}
	for (ClassStatConstIterator it = m_classes.begin(); it != m_classes.end(); ++it) {
		delete it->second;
	}
}

const FuncStat* ClassStat::getMethodStatByName(const char* funcNameId) const
{
	for (size_t i = 0; i != m_methodStats.size(); ++i) {
		if (m_methodStats[i]->getName().compare(funcNameId) == 0) {
			return m_methodStats[i];
		}
	}
	return NULL;
}

const ClassStat* ClassStat::getClassByName(const char* className) const
{
	ClassStatConstIterator it = m_classes.find(className);

	if (it != m_classes.end()) {
		return (*it).second;
	}
	return NULL;
}

ClassStat& ClassStat::GetOrCreateClass(const std::string& className)
{
	ClassStatConstIterator it = m_classes.find(className);

	if (it != m_classes.end()) {
		return *(*it).second;
	}
	ClassStat* classStat = new ClassStat(className, this, NULL);
	m_classes.insert(make_pair(className, classStat));
	return *classStat;
}

FuncStat* ClassStat::AddMethodStat(const std::string& className, unsigned int line)
{
	FuncStat* stat = new FuncStat(className, line);

	m_methodStats.push_back(stat);
	return stat;
}

} // namespace ccccc
