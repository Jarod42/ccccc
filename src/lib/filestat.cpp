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

#include "filestat.h"

namespace ccccc
{

FileStat::FileStat(const std::string& filename) :
	m_filename(filename),
	m_root("", NULL)
{
}

FileStat::~FileStat()
{
}

FuncStat* FileStat::AddFuncStat(const std::vector<std::string>& namespaceNames, const std::vector<std::string>& classeNames, const std::string& funcname, unsigned int line)
{
	NamespaceStat* namespaceStat = &m_root;
	for (size_t i = 0; i != namespaceNames.size(); ++i) {
		namespaceStat = &namespaceStat->GetOrCreateNamespace(namespaceNames[i]);
	}
	return namespaceStat->AddFuncStat(classeNames, funcname, line);
}

} // namespace ccccc
