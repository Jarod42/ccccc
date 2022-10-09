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

#ifndef UTILS_H
#define UTILS_H

#include <clang-c/Index.h>

#include <filesystem>
#include <string>

namespace ccccc::use_clang
{

std::string getStringAndDispose(CXString cxStr);
CXCursor getCursor(const CXTranslationUnit& tu, const CXToken& token);
unsigned int getStartLine(CXSourceRange range);
void getStartEndLine(CXSourceRange range, unsigned* startLine, unsigned* endLine);
void getStartEndOffset(CXSourceRange range, unsigned* startoffset, unsigned* endOffset);

bool isInFile(const std::filesystem::path& filename, CXCursor cursor);

bool isValid(CXTranslationUnit tu);

} // namespace ccccc::use_clang

#endif // UTILS_H
