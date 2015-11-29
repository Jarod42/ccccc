/*
** Copyright 2015 Joris Dauphin
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

#ifndef CALLER_COUNT_H
#define CALLER_COUNT_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <tuple>

#include <clang-c/Index.h>

namespace ccccc
{

struct CXCursorHash
{
	int operator ()(const CXCursor& cursor) const
	{
		return clang_hashCursor(cursor);
	}
};

struct CXCursorEqual
{
	bool operator ()(const CXCursor& lhs, const CXCursor& rhs) const
	{
		return clang_equalCursors(lhs, rhs);
	}
};

struct CallerCountData
{
	//  A map between Unified Symbol Resolution (USR) (a string
	// that identifies a particular entity)
	// and the number of time function/method is called.
	std::unordered_map<std::string, std::size_t> m_usrCounts;
	std::unordered_map<CXCursor, std::size_t, CXCursorHash, CXCursorEqual> m_counts;

	std::unordered_set<CXCursor, CXCursorHash, CXCursorEqual> m_templatedSpecializations;
	std::unordered_set<CXCursor, CXCursorHash, CXCursorEqual> m_templatedVisited;
};

}

#endif // CALLER_COUNT_H
