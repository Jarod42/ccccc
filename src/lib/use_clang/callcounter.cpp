/*
** Copyright 2022 Joris Dauphin
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

#include "callcounter.h"

namespace ccccc::use_clang
{

namespace
{

enum CXChildVisitResult
CallCounterVisitor(CXCursor cursor, [[maybe_unused]] CXCursor parent, CXClientData user_data)
{
	std::size_t& callCount = *reinterpret_cast<std::size_t*>(user_data);
	const auto cursorKind = clang_getCursorKind(cursor);

	if (cursorKind == CXCursorKind::CXCursor_CallExpr
	    || cursorKind == CXCursorKind::CXCursor_CXXDeleteExpr) {
		callCount++;
	}
	return CXChildVisitResult::CXChildVisit_Recurse;
}

} // anonymous namespace

std::size_t CountCall(const CXCursor& cursor)
{
	std::size_t count = 0;
	clang_visitChildren(cursor, &CallCounterVisitor, &count);
	return count;
}

} // namespace ccccc::use_clang
