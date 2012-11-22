
#ifndef CLASS_STAT_H
#define CLASS_STAT_H

#include <map>
#include <string>
#include <vector>

class FuncStat;
class NamespaceStat;

class ClassStat
{
	friend class NamespaceStat;
public:
	typedef std::map<std::string, ClassStat*> ClassMap;
	typedef ClassMap::const_iterator ClassStatConstIterator;
public:
	ClassStat(const std::string& name, ClassStat* classParent, NamespaceStat* namespaceParent);
	~ClassStat();

	const std::string& getName() const { return m_name; }
	unsigned int getMethodCount() const { return m_methodStats.size(); }
	const FuncStat& getMethodStat(unsigned int index) const { return *m_methodStats[index]; }
	const FuncStat* getMethodStatByName(const char* funcNameId) const;

	unsigned int getClassCount() const { return m_classes.size(); }
	ClassStatConstIterator getClass_begin() const { return m_classes.begin(); }
	ClassStatConstIterator getClass_end() const { return m_classes.end(); }
	const ClassStat* getClassByName(const char* funcNameId) const;

private:
	FuncStat* AddMethodStat(const std::string& funcname, unsigned int line);
	ClassStat& GetOrCreateClass(const std::string& funcname);
private:
	std::string m_name;
	std::vector<FuncStat*> m_methodStats;
	ClassMap m_classes;
	NamespaceStat* m_namespaceParent;
	ClassStat* m_classParent;
};


#endif // CLASS_STAT_H
