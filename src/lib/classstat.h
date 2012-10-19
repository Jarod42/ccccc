
#ifndef CLASS_STAT_H
#define CLASS_STAT_H

#include <map>
#include <string>
#include <vector>

class FuncStat;

class ClassStat
{
	friend class FuncStatTool;
	friend class FileStat;
public:
	typedef std::map<std::string, ClassStat*> ClassMap;
	typedef ClassMap::const_iterator ClassStatConstIterator;
public:
	ClassStat(const std::string& name, NamespaceStat* parent);
	~ClassStat();

	const std::string& getName() const { return m_name; }
	unsigned int getMethodCount() const { return m_methodStats.size(); }
	const FuncStat& getMethodStat(unsigned int index) const { return *m_methodStats[index]; }
	const FuncStat* getMethodStatByName(const char *funcNameId) const;

	unsigned int getNamespaceCount() const { return m_funcStats.size(); }
	ClassStatConstIterator getClass_begin() const { return m_namespaces.begin(); }
	ClassStatConstIterator getClass_end() const { return m_namespaces.end(); }
	const ClassStat* getClassByName(const char *funcNameId) const;

private:
	FuncStat* AddFuncStat(const std::string& funcname);
	NamespaceStat& GetOrCreateNamespace(const std::string& funcname);
private:
	std::string m_name;
	std::vector<FuncStat*> m_methodStats;
	NamespaceMap m_namespaces;
	NamespaceStat* m_parent;
	ClassStat* m_parentClass;
};


#endif // CLASS_STAT_H
