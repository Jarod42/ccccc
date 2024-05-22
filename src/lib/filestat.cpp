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

#include "filestat.h"

namespace ccccc
{

FileStat::FileStat(const std::filesystem::path& filename) :
	m_filename(filename),
	m_root("", nullptr)
{}

FileStat::~FileStat() = default;

FuncStat* FileStat::AddFuncStat(const std::vector<std::string>& namespaceNames,
                                const std::vector<std::string>& classeNames,
                                const std::string& funcname,
                                std::size_t line)
{
	NamespaceStat* namespaceStat = &m_root;
	for (const auto& namespaceName : namespaceNames) {
		namespaceStat = &namespaceStat->GetOrCreateNamespace(namespaceName);
	}
	return namespaceStat->AddFuncStat(classeNames, funcname, line);
}

} // namespace ccccc
