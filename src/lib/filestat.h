
#ifndef FILE_STAT_H
#define FILE_STAT_H

#include "namespacestat.h"
#include "localstat.h"
#include "funcstat.h"

#include <string>
#include <vector>

class FileStat
{
	friend class FileStatTool;
public:
	typedef NamespaceStat::NamespaceStatConstIterator NamespaceStatConstIterator;
	typedef NamespaceStat::ClassStatConstIterator ClassStatConstIterator;
public:
	explicit FileStat(const std::string& filename);
	~FileStat();

	const std::string& getFilename() const { return m_filename; }
	const LocalStat& getStat() const { return m_stat; }
	unsigned int getFunctionCount() const { return m_root.getFunctionCount(); }
	const FuncStat& getFuncStat(unsigned int index) const { return m_root.getFuncStat(index); }
	const FuncStat* getFuncStatByName(const char *funcNameId) const { return m_root.getFuncStatByName(funcNameId); }

	unsigned int getNamespaceCount() const { return m_root.getNamespaceCount(); }
	NamespaceStatConstIterator getNamespace_begin() const { return m_root.getNamespace_begin(); }
	NamespaceStatConstIterator getNamespace_end() const { return m_root.getNamespace_end(); }
	const NamespaceStat* getNamespaceByName(const char *name) const { return m_root.getNamespaceByName(name); }

	unsigned int getClassCount() const { return m_root.getClassCount(); }
	ClassStatConstIterator getClass_begin() const { return m_root.getClass_begin(); }
	ClassStatConstIterator getClass_end() const { return m_root.getClass_end(); }
	const ClassStat* getClassByName(const char *name) const { return m_root.getClassByName(name); }
private:
	FuncStat* AddFuncStat(const std::vector<std::string>& namespaceNames, const std::vector<std::string>& classeNames, const std::string& funcname, unsigned int line);
	NamespaceStat* AddNamespace(const std::string& name);
private:
	std::string m_filename;
	LocalStat m_stat;
	NamespaceStat m_root;
};


#endif // FILE_STAT_H
