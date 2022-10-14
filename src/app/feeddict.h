/*
** Copyright 2012-2022 Joris Dauphin
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

#include <filesystem>
#include <string>

namespace ccccc
{
class ClassStat;
class FileStat;
class FuncStat;
class NamespaceStat;
} // namespace ccccc

namespace ctemplate
{
class TemplateDictionary;
} // namespace ctemplate

void feedDict(const ccccc::FuncStat&,
              const std::string& namespacesName,
              const std::string& classesName,
              ctemplate::TemplateDictionary*);
void feedDict(const ccccc::ClassStat&,
              const std::string& namespacesName,
              std::string classesName,
              ctemplate::TemplateDictionary*);
void feedDict(const ccccc::NamespaceStat&,
              std::string namespacesName,
              ctemplate::TemplateDictionary*);
void feedDict(const ccccc::FileStat&,
              const std::filesystem::path& root,
              ctemplate::TemplateDictionary*);

#endif // FEEDDICT_H
