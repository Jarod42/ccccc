
#include <clang-c/Index.h>
#include <assert.h>
#include <iostream>

#include "localstat.h"
#include "utils.h"

template <typename STREAM>
STREAM & operator << (STREAM& s, const LocalStat& localStat)
{
	return s << "LOCphy=" << localStat.getLineOfCode_physic()
			<< "|LOCpro=" << localStat.getLineOfCode_program()
			<< "|LOCcom=" << localStat.getLineOfCode_comment()
			<< "|LOCbl="  << localStat.getLineOfCode_blank()
			<< "|mvg=" << localStat.getMcCabeCyclomaticNumber();
}


class ClientData
{
public:
	explicit ClientData(CXTranslationUnit tu) : m_tu(tu) {}

	CXTranslationUnit getCXTranslationUnit() { return m_tu; }

private:
	CXTranslationUnit m_tu;
};

void output(CXSourceLocation loc)
{
	CXFile file;
	unsigned line;
	unsigned column;
	unsigned offset;
	clang_getExpansionLocation(loc, &file, &line, &column, &offset);
	std::cout << line << "," << column;
}

void output(CXSourceRange range)
{
	std::cout << "[";
	CXSourceLocation start = clang_getRangeStart(range);
	CXSourceLocation end = clang_getRangeEnd(range);
	output(start);
	std::cout << "-";
	output(end);
	std::cout << "]";
}

void output(CXCursor cursor)
{
//	CXString kind = clang_getCursorKindSpelling(clang_getCursorKind(cursor));
//	CXString usr = clang_getCursorUSR(cursor);
//	CXString spelling = clang_getCursorSpelling(cursor);
	CXString displayName = clang_getCursorDisplayName(cursor);

	std::cout //<< clang_getCString(kind) << "("
//		<< clang_getCString(usr) << " - "
//		<< clang_getCString(spelling)
//		<< ":"
		<< clang_getCString(displayName)
//		<< ")"
	;

//	CXSourceRange range = clang_getCursorExtent(cursor);
//	output(range);

	clang_disposeString(displayName);
//	clang_disposeString(spelling);
//	clang_disposeString(usr);
//	clang_disposeString(kind);
}

enum CXChildVisitResult MyCursorVisitor(CXCursor cursor,
                                        CXCursor parent,
                                        CXClientData user_data)
{
	if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl
		&& clang_isCursorDefinition(cursor)) {
		ClientData* client_data = reinterpret_cast<ClientData*>(user_data);

		output(cursor);
		LocalStat localStat;
		localStat.Compute(client_data->getCXTranslationUnit(), cursor);
		std::cout << ":" << localStat;
		std::cout << std::endl;
	}
	return CXChildVisit_Recurse;
}

void dummyTest(const char* filename)
{
	CXIndex index = clang_createIndex(1, 1);

	// Support Microsoft extensions
	const char *args[] = {"-fms-extensions"};

	CXTranslationUnit tu = clang_parseTranslationUnit(index, filename, args, ARRAY_SIZE(args), 0, 0, 0);

	if (tu)
	{
		CXCursor cursor = clang_getTranslationUnitCursor(tu);

		ClientData clientData(tu);
		clang_visitChildren(cursor, MyCursorVisitor, &clientData);

		LocalStat localStat;
		localStat.Compute(tu, cursor);
		std::cout << localStat;
	}
	clang_disposeTranslationUnit(tu);
	clang_disposeIndex(index);
}


int main()
{
	dummyTest("test.c");
	return 0;
}

