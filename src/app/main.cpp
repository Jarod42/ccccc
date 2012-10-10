
#include <clang-c/Index.h>
#include <assert.h>
#include <iostream>
#include <vector>

#include "localstat.h"
#include "use_clang/utils.h"
#include "cccc_clang_api.h"

#if 0
class namespaceStat
{

};

class classStat
{

};
#endif


template <typename STREAM>
STREAM & operator << (STREAM& s, const LocalStat& localStat)
{
	return s //<< localStat.getModuleName() << ":"
			<< "LOCphy=" << localStat.getLineOfCode_physic()
			<< "|LOCpro=" << localStat.getLineOfCode_program()
			<< "|LOCcom=" << localStat.getLineOfCode_comment()
			<< "|LOCbl="  << localStat.getLineOfCode_blank()
			<< "|mvg=" << localStat.getMcCabeCyclomaticNumber();
}

template <typename STREAM>
STREAM & operator << (STREAM& s, const FuncStat& funcStat)
{
	return s << funcStat.getName() << ":" << funcStat.getStat();
}

template <typename STREAM>
STREAM & operator << (STREAM& s, const FileStat& fileStat)
{
	s << fileStat.getFilename() << std::endl;
	s << fileStat.getStat() << std::endl;

	for (unsigned int i = 0; i != fileStat.getFunctionCount(); ++i) {
		const FuncStat& funcStat = fileStat.getFuncStat(i);
		s << "\t" << funcStat << std::endl;
	}
	return s;
}



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

int main(int argc, char* argv[])
{
	if (argc > 1) {
		ccccc::Parameters params;

		params.InitHardCodedMingwPath();
		params.Parse(argc, argv);
		ccccc::AllStat allStat;

		allStat.Compute(params);

		for (unsigned int i = 0; i != allStat.getFileCount(); ++i) {
			const FileStat& filestat = allStat.getFileStat(i);

			std::cout << filestat;
		}
	}
	return 0;
}

