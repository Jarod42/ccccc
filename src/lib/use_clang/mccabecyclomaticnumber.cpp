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

#include "mccabecyclomaticnumber.h"

#include <clang-c/Index.h>

#include "utils.h"

namespace ccccc
{
namespace use_clang
{

McCabeCyclomaticNumber::McCabeCyclomaticNumber() : m_value(1)
{
}

void McCabeCyclomaticNumber::operator()(const CXTranslationUnit& tu, const CXCursor& cursor, const CXToken& token)
{
	if (clang_getTokenKind(token) == CXToken_Keyword) {
		const std::string str = getStringAndDispose(clang_getTokenSpelling(tu, token));
		const char* keywords[] = {"if", "for", "while", "case", "catch"}; // #if, #ifdef, #ifndef, #elif

		for (unsigned int i = 0; i != ARRAY_SIZE(keywords); ++i) {
			if (str.compare(keywords[i]) == 0) {
				++m_value;
				break;
			}
		}
	} else if (clang_getTokenKind(token) == CXToken_Punctuation) {
		const std::string str = getStringAndDispose(clang_getTokenSpelling(tu, token));
		const char* keywords[] = {"?", "&&", "||"};

		for (unsigned int i = 0; i != ARRAY_SIZE(keywords); ++i) {
			if (str.compare(keywords[i]) == 0) {
				CXCursor cursor = getCursor(tu, token);

				if (clang_isDeclaration(clang_getCursorKind(cursor)) == false) { // && can be part of declaration in C++11
					++m_value;
				}
				break;
			}
		}
	}
}

} // namespace use_clang
} // namespace ccccc
