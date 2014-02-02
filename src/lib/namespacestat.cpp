/*
** Copyright 2012 Joris Dauphin
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
	return nullptr;
}

const NamespaceStat* NamespaceStat::getNamespaceByName(const char* namespaceName) const
{
	NamespaceStatConstIterator it = m_namespaces.find(namespaceName);

	if (it != m_namespaces.end()) {
		return (*it).second;
	}
	return nullptr;
}

const ClassStat* NamespaceStat::getClassByName(const char* ClassName) const
{
	ClassStatConstIterator it = m_classes.find(ClassName);

	if (it != m_classes.end()) {
		return (*it).second;
	}
	return nullptr;
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
	ClassStat* classStat = new ClassStat(className, nullptr, this);
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
