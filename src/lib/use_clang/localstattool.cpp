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

#include "localstattool.h"

#include <clang-c/Index.h>

#include <string>
#include "utils.h"

#include <assert.h>

namespace ccccc
{
namespace use_clang
{

static CXCursor getCursor(const CXTranslationUnit& tu, const CXToken& token)
{
	return clang_getCursor(tu, clang_getTokenLocation(tu, token));
}

static void updateMcCabeCyclomaticNumber(const CXTranslationUnit& tu, const CXToken& token, unsigned int* mcCabeCyclomaticNumber)
{
	if (clang_getTokenKind(token) == CXToken_Keyword) {
		CXString cxstr = clang_getTokenSpelling(tu, token);
		std::string str = clang_getCString(cxstr);
		clang_disposeString(cxstr);
		const char* keywords[] = {"if", "for", "while", "case", "catch"}; // #if, #ifdef, #ifndef, #elif

		for (unsigned int i = 0; i != ARRAY_SIZE(keywords); ++i) {
			if (str.compare(keywords[i]) == 0) {
				++*mcCabeCyclomaticNumber;
				break;
			}
		}
	} else if (clang_getTokenKind(token) == CXToken_Punctuation) {
		CXString cxstr = clang_getTokenSpelling(tu, token);
		std::string str = clang_getCString(cxstr);
		clang_disposeString(cxstr);

		const char* keywords[] = {"?", "&&", "||"};

		for (unsigned int i = 0; i != ARRAY_SIZE(keywords); ++i) {
			if (str.compare(keywords[i]) == 0) {
				CXCursor cursor = getCursor(tu, token);

				if (clang_isDeclaration(clang_getCursorKind(cursor)) == false) { // && can be part of declaration in C++11
					++*mcCabeCyclomaticNumber;
				}
				break;
			}
		}
	}
}

void LocalStatTool::Compute(const CXTranslationUnit& tu, const CXCursor& cursor, LineCount* lineCount)
{
	CXToken* tokens;
	unsigned numToken;
	clang_tokenize(tu, clang_getCursorExtent(cursor), &tokens, &numToken);

	unsigned cursorStartLine, cursorEndLine;
	getStartEndLine(clang_getCursorExtent(cursor), &cursorStartLine, &cursorEndLine);
	lineCount->lineOfCode_physic = cursorEndLine - cursorStartLine + 1;

	assert(cursorStartLine != 0);
	// com, loc, blankLine
	unsigned int lastLine[2] = {cursorStartLine - 1, cursorStartLine - 1};
	unsigned int lineOfCode_physic[3] = {0, 0, 0};
	for (unsigned i = 0; i != numToken; ++i) {
		unsigned startLine, endLine;
		updateMcCabeCyclomaticNumber(tu, tokens[i], &lineCount->mcCabeCyclomaticNumber);
		getStartEndLine(clang_getTokenExtent(tu, tokens[i]), &startLine, &endLine);
		unsigned int type = (clang_getTokenKind(tokens[i]) == CXToken_Comment) ? 0 : 1;

		if (startLine > cursorEndLine) {
			break;
		}
		endLine = std::min(endLine, cursorEndLine);

		if (startLine != lastLine[type]) {
			lineOfCode_physic[type] += endLine - startLine + 1;
			unsigned int maxLast = std::max(lastLine[0], lastLine[1]);
			if (startLine > maxLast) {
				lineOfCode_physic[2] += startLine - maxLast - 1;
			}
		} else {
			lineOfCode_physic[type] += endLine - startLine;
		}
		lastLine[type] = endLine;
	}
	clang_disposeTokens(tu, tokens, numToken);
	lineCount->lineOfCode_comment = lineOfCode_physic[0];
	lineCount->lineOfCode_program = lineOfCode_physic[1];
	lineCount->lineOfCode_blank = lineOfCode_physic[2];
}

} // namespace use_clang
} // namespace ccccc
