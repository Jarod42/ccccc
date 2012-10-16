
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

private:
	FuncStat* AddFuncStat(const std::string& funcname, const std::vector<std::string>& namespaceNames);
	NamespaceStat* AddNamespace(const std::string& funcname);
private:
	std::string m_filename;
	LocalStat m_stat;
	NamespaceStat m_root;
};


#endif // FILE_STAT_H
