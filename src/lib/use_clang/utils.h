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

#ifndef UTILS_H
#define UTILS_H

#define ARRAY_SIZE(a) (sizeof (a) / sizeof(*a))

#include <clang-c/Index.h>
#include <string>

namespace ccccc
{
namespace use_clang
{


unsigned int getStartLine(CXSourceRange range);
void getStartEndLine(CXSourceRange range, unsigned* startLine, unsigned* endLine);

std::string getStringAndDispose(CXString cxStr);

bool isInFile(const char* filename, CXCursor cursor);

bool isValid(CXTranslationUnit tu);

} // namespace use_clang
} // namespace ccccc

#endif // UTILS_H
