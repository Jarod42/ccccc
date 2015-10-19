/*
** Copyright 2012-2015 Joris Dauphin
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

#ifndef CLASS_STAT_H
#define CLASS_STAT_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ccccc
{

class FuncStat;
class NamespaceStat;
namespace use_clang
{
	class FileStatTool;
}

class ClassStat
{
	friend class use_clang::FileStatTool;
	friend class NamespaceStat;
public:
	using ClassMap = std::map<std::string, std::unique_ptr<ClassStat>>;
public:
	ClassStat(const std::string& name, ClassStat* classParent, NamespaceStat* namespaceParent);
	~ClassStat();

	const std::string& getName() const { return m_name; }
	unsigned int getMethodCount() const { return m_methodStats.size(); }
	const FuncStat& getMethodStat(unsigned int index) const { return *m_methodStats[index]; }
	const FuncStat* getMethodStatByName(const char* funcNameId) const;

	unsigned int getClassCount() const { return m_classes.size(); }
	const ClassMap& getInnerClasses() const { return m_classes; }
	const ClassStat* getClassByName(const char* funcNameId) const;

private:
	FuncStat* AddMethodStat(const std::string& funcname, unsigned int line);
	ClassStat& GetOrCreateClass(const std::string& funcname);
private:
	std::string m_name;
	std::vector<std::unique_ptr<FuncStat>> m_methodStats;
	ClassMap m_classes;
	NamespaceStat* m_namespaceParent;
	ClassStat* m_classParent;
};

} // namespace ccccc

#endif // CLASS_STAT_H
