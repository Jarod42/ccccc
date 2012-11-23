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

#ifndef LOCALSTATTOOL_H
#define LOCALSTATTOOL_H

#include "../localstat.h"
#include <clang-c/Index.h>

namespace ccccc
{
namespace use_clang
{

class LocalStatTool
{
public:
	static void Compute(const CXTranslationUnit& tu, const CXCursor& cursor, LocalStat* stat);
private:
	static void UpdateMcCabeCyclomaticNumber(const CXTranslationUnit& tu, const CXToken& token, LocalStat* stat);
};

} // namespace use_clang
} // namespace ccccc

#endif // LOCALSTATTOOL_H
