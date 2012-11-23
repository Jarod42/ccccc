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
