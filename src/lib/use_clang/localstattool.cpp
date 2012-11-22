

#include "localstattool.h"

#include <clang-c/Index.h>

#include <string>
#include "utils.h"

#include <assert.h>

static CXCursor getCursor(const CXTranslationUnit& tu, const CXToken& token)
{
	return clang_getCursor(tu, clang_getTokenLocation(tu, token));
}

void LocalStatTool::UpdateMcCabeCyclomaticNumber(const CXTranslationUnit& tu, const CXToken& token, LocalStat* stat)
{
	if (clang_getTokenKind(token) == CXToken_Keyword) {
		CXString cxstr = clang_getTokenSpelling(tu, token);
		std::string str = clang_getCString(cxstr);
		clang_disposeString(cxstr);
		const char* keywords[] = {"if", "for", "while", "case", "catch"}; // #if, #ifdef, #ifndef, #elif

		for (unsigned int i = 0; i != ARRAY_SIZE(keywords); ++i) {
			if (str.compare(keywords[i]) == 0) {
				++stat->mcCabeCyclomaticNumber;
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
					++stat->mcCabeCyclomaticNumber;
				}
				break;
			}
		}
	}
}

void LocalStatTool::Compute(const CXTranslationUnit& tu, const CXCursor& cursor, LocalStat* stat)
{
	CXToken* tokens;
	unsigned numToken;
	clang_tokenize(tu, clang_getCursorExtent(cursor), &tokens, &numToken);

	unsigned cursorStartLine, cursorEndLine;
	getStartEndLine(clang_getCursorExtent(cursor), &cursorStartLine, &cursorEndLine);
	stat->lineOfCode_physic = cursorEndLine - cursorStartLine + 1;

	assert(cursorStartLine != 0);
	// com, loc, blankLine
	unsigned int lastLine[2] = {cursorStartLine - 1, cursorStartLine - 1};
	unsigned int lineOfCode_physic[3] = {0, 0, 0};
	for (unsigned i = 0; i != numToken; ++i) {
		unsigned startLine, endLine;
		LocalStatTool::UpdateMcCabeCyclomaticNumber(tu, tokens[i], stat);
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
	stat->lineOfCode_comment = lineOfCode_physic[0];
	stat->lineOfCode_program = lineOfCode_physic[1];
	stat->lineOfCode_blank = lineOfCode_physic[2];
}
