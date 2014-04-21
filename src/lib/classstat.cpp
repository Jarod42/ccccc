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
}

const FuncStat* ClassStat::getMethodStatByName(const char* funcNameId) const
{
	for (const auto& methodStat : m_methodStats) {
		if (methodStat->getName().compare(funcNameId) == 0) {
			return methodStat.get();
		}
	}
	return nullptr;
}

const ClassStat* ClassStat::getClassByName(const char* className) const
{
	ClassStatConstIterator it = m_classes.find(className);

	if (it != m_classes.end()) {
		return (*it).second.get();
	}
	return nullptr;
}

ClassStat& ClassStat::GetOrCreateClass(const std::string& className)
{
	ClassStatConstIterator it = m_classes.find(className);

	if (it != m_classes.end()) {
		return *(*it).second;
	}
	std::unique_ptr<ClassStat> classStat(new ClassStat(className, this, nullptr));
	return *m_classes.insert(make_pair(className, std::move(classStat))).first->second;
}

FuncStat* ClassStat::AddMethodStat(const std::string& className, unsigned int line)
{
	std::unique_ptr<FuncStat> stat(new FuncStat(className, line));

	m_methodStats.push_back(std::move(stat));
	return m_methodStats.back().get();
}

} // namespace ccccc
