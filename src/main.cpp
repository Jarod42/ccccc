
#include <clang-c/Index.h>
#include <assert.h>
#include <iostream>

#include "localstat.h"
#include "utils.h"

template <typename STREAM>
STREAM & operator << (STREAM& s, const LocalStat& localStat)
{
	return s << localStat.getModuleName() << ":"
			<< "LOCphy=" << localStat.getLineOfCode_physic()
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
//	std::string kind = getStringAndDispose(clang_getCursorKindSpelling(clang_getCursorKind(cursor)));
//	std::string usr = getStringAndDispose(clang_getCursorUSR(cursor));
//	std::string spelling = getStringAndDispose(clang_getCursorSpelling(cursor));
	std::string displayName = getStringAndDispose(clang_getCursorDisplayName(cursor));

	std::cout //<< kind << "("
//		<< usr << " - "
//		<< spelling
//		<< ":"
		<< displayName
//		<< ")"
	;

//	CXSourceRange range = clang_getCursorExtent(cursor);
//	output(range);
}

enum CXChildVisitResult MyCursorVisitor(CXCursor cursor,
                                        CXCursor parent,
                                        CXClientData user_data)
{
	if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl
		&& clang_isCursorDefinition(cursor)) {
		ClientData* client_data = reinterpret_cast<ClientData*>(user_data);

		//output(cursor);
		LocalStat localStat(getStringAndDispose(clang_getCursorDisplayName(cursor)));
		localStat.Compute(client_data->getCXTranslationUnit(), cursor);
		std::cout << localStat << std::endl;
	}
	return CXChildVisit_Recurse;
}

void dummyTest(const char* filename)
{
	CXIndex index = clang_createIndex(1, 1);

	// Hard coded system headersHard coded
	#define MINGWPATH "d:/Programs/mingw-4.6.1"
	#define MINGWPATH2 MINGWPATH"/lib/gcc/mingw32/4.6.1"
	const char *args[] = {
		"-I"MINGWPATH"/include",
		"-I"MINGWPATH2"/include/c++",
		"-I"MINGWPATH2"/include/c++/mingw32",
		"-I"MINGWPATH2"/include/c++/backward",
		"-I"MINGWPATH2"/include",
		"-I"MINGWPATH2"/include-fixed",
	};

	CXTranslationUnit tu = clang_parseTranslationUnit(index, filename, args, ARRAY_SIZE(args), 0, 0, 0);

	if (tu)
	{
		CXCursor cursor = clang_getTranslationUnitCursor(tu);

		LocalStat localStat(filename);
		localStat.Compute(tu, cursor);
		std::cout << localStat << std::endl;

		ClientData clientData(tu);
		clang_visitChildren(cursor, MyCursorVisitor, &clientData);
	}
	clang_disposeTranslationUnit(tu);
	clang_disposeIndex(index);
}


int main(int argc, char* argv[])
{
	if (argc > 1) {
		dummyTest(argv[1]);
	}
	return 0;
}

