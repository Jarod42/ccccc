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
}

const FuncStat* NamespaceStat::getFuncStatByName(const char* funcNameId) const
{
	for (const auto& funcStat : m_funcStats) {
		if (funcStat->getName().compare(funcNameId) == 0) {
			return funcStat.get();
		}
	}
	return nullptr;
}

const NamespaceStat* NamespaceStat::getNamespaceByName(const char* namespaceName) const
{
	auto it = m_namespaces.find(namespaceName);

	if (it != m_namespaces.end()) {
		return (*it).second.get();
	}
	return nullptr;
}

const ClassStat* NamespaceStat::getClassByName(const char* ClassName) const
{
	auto it = m_classes.find(ClassName);

	if (it != m_classes.end()) {
		return (*it).second.get();
	}
	return nullptr;
}

NamespaceStat& NamespaceStat::GetOrCreateNamespace(const std::string& namespaceName)
{
	auto it = m_namespaces.find(namespaceName);

	if (it != m_namespaces.end()) {
		return *(*it).second;
	}
	std::unique_ptr<NamespaceStat> namespaceStat(new NamespaceStat(namespaceName, this));
	return *m_namespaces.insert(make_pair(namespaceName, std::move(namespaceStat))).first->second;
}

ClassStat& NamespaceStat::GetOrCreateClass(const std::string& className)
{
	ClassStatConstIterator it = m_classes.find(className);

	if (it != m_classes.end()) {
		return *(*it).second;
	}
	std::unique_ptr<ClassStat> classStat(new ClassStat(className, nullptr, this));
	return *m_classes.insert(make_pair(className, std::move(classStat))).first->second;
}

FuncStat* NamespaceStat::AddFuncStat(const std::vector<std::string>& classeNames, const std::string& funcname, unsigned int line)
{
	if (classeNames.empty()) {
		std::unique_ptr<FuncStat> stat(new FuncStat(funcname, line));

		m_funcStats.push_back(std::move(stat));
		return m_funcStats.back().get();
	}
	ClassStat* classStat = &GetOrCreateClass(classeNames[0]);
	for (size_t i = 1; i != classeNames.size(); ++i) {
		classStat = &classStat->GetOrCreateClass(classeNames[i]);
	}
	return classStat->AddMethodStat(funcname, line);
}

} // namespace ccccc
