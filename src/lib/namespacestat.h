/*
** Copyright 2012-2014 Joris Dauphin
*/
/*
**  This file is part of CCCCC.
**
**  CCCCC is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  CCCCC is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with CCCCC. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NAMESPACE_STAT_H
#define NAMESPACE_STAT_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ccccc
{

class FuncStat;
class ClassStat;

class NamespaceStat
{
	friend class FuncStatTool;
	friend class FileStat;
public:
	typedef std::map<std::string, std::unique_ptr<NamespaceStat>> NamespaceMap;
	typedef NamespaceMap::const_iterator NamespaceStatConstIterator;
	typedef std::map<std::string, std::unique_ptr<ClassStat>> ClassMap;
	typedef ClassMap::const_iterator ClassStatConstIterator;
public:
	NamespaceStat(const std::string& name, NamespaceStat* parent);
	~NamespaceStat();

	const std::string& getName() const { return m_name; }
	unsigned int getFunctionCount() const { return m_funcStats.size(); }
	const FuncStat& getFuncStat(unsigned int index) const { return *m_funcStats[index]; }
	const FuncStat* getFuncStatByName(const char* funcNameId) const;

	unsigned int getNamespaceCount() const { return m_namespaces.size(); }
	NamespaceStatConstIterator getNamespace_begin() const { return m_namespaces.begin(); }
	NamespaceStatConstIterator getNamespace_end() const { return m_namespaces.end(); }
	const NamespaceStat* getNamespaceByName(const char* name) const;

	unsigned int getClassCount() const { return m_classes.size(); }
	ClassStatConstIterator getClass_begin() const { return m_classes.begin(); }
	ClassStatConstIterator getClass_end() const { return m_classes.end(); }
	const ClassStat* getClassByName(const char* name) const;
private:
	FuncStat* AddFuncStat(const std::vector<std::string>& classeNames, const std::string& funcname, unsigned int line);
	NamespaceStat& GetOrCreateNamespace(const std::string& name);
	ClassStat& GetOrCreateClass(const std::string& className);
private:
	std::string m_name;
	std::vector<std::unique_ptr<FuncStat>> m_funcStats;
	NamespaceMap m_namespaces;
	ClassMap m_classes;
	NamespaceStat* m_parent;
};

} // namespace ccccc

#endif // NAMESPACE_STAT_H
