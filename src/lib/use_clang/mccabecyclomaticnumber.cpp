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

#include "mccabecyclomaticnumber.h"

#include "utils.h"

#include <clang-c/Index.h>

#include <algorithm>

namespace ccccc::use_clang
{

void McCabeCyclomaticNumber::operator()(const CXTranslationUnit& tu, const CXCursor& /*cursor*/, const CXToken& token)
{
	if (clang_getTokenKind(token) == CXToken_Keyword) {
		const std::string str = getStringAndDispose(clang_getTokenSpelling(tu, token));
		const char* keywords[] = {"if", "for", "while", "case", "catch"}; // #if, #ifdef, #ifndef, #elif

		if (std::find(std::begin(keywords), std::end(keywords), str) != std::end(keywords)) {
			++m_value;
		}
	} else if (clang_getTokenKind(token) == CXToken_Punctuation) {
		const std::string str = getStringAndDispose(clang_getTokenSpelling(tu, token));
		const char* keywords[] = {"?", "&&", "||"};

		if (std::find(std::begin(keywords), std::end(keywords), str) != std::end(keywords)) {
			CXCursor cursor = getCursor(tu, token);

			if (clang_isDeclaration(clang_getCursorKind(cursor)) == false) { // && can be part of declaration in C++11
				++m_value;
			}
		}
	}
}

} // namespace ccccc::use_clang
