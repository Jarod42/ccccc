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

#include "../linecount.h"
#include <clang-c/Index.h>
#include "utils.h"

namespace ccccc
{
namespace use_clang
{

template <typename FUNC>
static void processTokens(const CXTranslationUnit& tu, const CXCursor& cursor, FUNC& f)
{
	CXToken* tokens;
	unsigned numToken;
	clang_tokenize(tu, clang_getCursorExtent(cursor), &tokens, &numToken);
	unsigned cursorStartOffset, cursorEndOffset;
	getStartEndOffset(clang_getCursorExtent(cursor), &cursorStartOffset, &cursorEndOffset);

	unsigned int startIndex = 0;
	for (startIndex = 0; startIndex != numToken; ++startIndex) {
		unsigned startOffset, endOffset;
		getStartEndOffset(clang_getTokenExtent(tu, tokens[startIndex]), &startOffset, &endOffset);
		if (cursorStartOffset <= startOffset) {
			break;
		}
	}

	for (unsigned int i = startIndex; i != numToken; ++i) {
		const CXToken& token = tokens[i];
		unsigned startOffset, endOffset;
		getStartEndOffset(clang_getTokenExtent(tu, token), &startOffset, &endOffset);
		if (cursorEndOffset < endOffset) {
			break;
		}
		f(tu, cursor, token);
	}
	clang_disposeTokens(tu, tokens, numToken);
}

} // namespace use_clang
} // namespace ccccc

#endif // LOCALSTATTOOL_H
