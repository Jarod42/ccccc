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

#ifndef FUNCSTATTOOL_H
#define FUNCSTATTOOL_H

#include "../funcstat.h"

#include <clang-c/Index.h>

#include <filesystem>

namespace ccccc
{
struct GlobalData;
} // namespace ccccc

namespace ccccc::use_clang
{

class FuncStatTool
{
public:
	static void
	Compute(const std::filesystem::path&, const CXTranslationUnit&, const CXCursor&, FuncStat*);
	static void PostFeed(const GlobalData&, FuncStat*);
};

} // namespace ccccc::use_clang

#endif // FUNCSTATTOOL_H
