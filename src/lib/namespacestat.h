
#ifndef NAMESPACE_STAT_H
#define NAMESPACE_STAT_H

#include <map>
#include <string>
#include <vector>

class FuncStat;

class NamespaceStat
{
	friend class FuncStatTool;
	friend class FileStat;
public:
	typedef std::map<std::string, NamespaceStat*> NamespaceMap;
	typedef NamespaceMap::const_iterator NamespaceStatConstIterator;
public:
	NamespaceStat(const std::string& name, NamespaceStat* parent);
	~NamespaceStat();

	const std::string& getName() const { return m_name; }
	unsigned int getFunctionCount() const { return m_funcStats.size(); }
	const FuncStat& getFuncStat(unsigned int index) const { return *m_funcStats[index]; }
	const FuncStat* getFuncStatByName(const char *funcNameId) const;

	unsigned int getNamespaceCount() const { return m_funcStats.size(); }
	NamespaceStatConstIterator getNamespace_begin() const { return m_namespaces.begin(); }
	NamespaceStatConstIterator getNamespace_end() const { return m_namespaces.end(); }
	const NamespaceStat* getNamespaceByName(const char *funcNameId) const;

private:
	FuncStat* AddFuncStat(const std::string& funcname);
	NamespaceStat& GetOrCreateNamespace(const std::string& funcname);
private:
	std::string m_name;
	std::vector<FuncStat*> m_funcStats;
	NamespaceMap m_namespaces;
	NamespaceStat* m_parent;
};


#endif // NAMESPACE_STAT_H
