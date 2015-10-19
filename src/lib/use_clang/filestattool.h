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

#ifndef FILESTATTOOL_H
#define FILESTATTOOL_H

#include "../filestat.h"
#include <clang-c/Index.h>

namespace ccccc
{
	struct GlobalData;

namespace use_clang
{

class FileStatTool
{
public:
	static void Compute(const CXTranslationUnit& tu, GlobalData&, FileStat* stat);
	static void PostFeed(const GlobalData& globalData, FileStat* stat);
private:
	static void PostFeed(const GlobalData& globalData, NamespaceStat* stat);
	static void PostFeed(const GlobalData& globalData, ClassStat* stat);

	static enum CXChildVisitResult FileCursorVisitor(CXCursor cursor, CXCursor parent, CXClientData user_data);
	static void VisitNamespace(CXCursor cursor, CXClientData user_data);
};

} // namespace use_clang
} // namespace ccccc

#endif // FILESTATTOOL_H
