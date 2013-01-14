/*
** Copyright 2012-2013 Joris Dauphin
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


#ifndef FEEDDICT_H
#define FEEDDICT_H

#include <string>

namespace ccccc
{
class ClassStat;
class FileStat;
class FuncStat;
class NamespaceStat;
}

namespace ctemplate
{
class TemplateDictionary;
}

void feedDict(const ccccc::FuncStat& funcStat, const std::string& namespacesName, const std::string& classesName, ctemplate::TemplateDictionary* dict);
void feedDict(const ccccc::ClassStat& classStat, const std::string& namespacesName, std::string classesName, ctemplate::TemplateDictionary* dict);
void feedDict(const ccccc::NamespaceStat& namespaceStat, std::string namespacesName, ctemplate::TemplateDictionary* dict);
void feedDict(const ccccc::FileStat& fileStat, ctemplate::TemplateDictionary* dict);

#endif // FEEDDICT_H
