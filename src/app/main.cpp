
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

#if 1
// thank to James Kanze in http://stackoverflow.com/a/9600752

class IndentingOStreambuf : public std::streambuf
{
public:
	explicit IndentingOStreambuf(std::streambuf* dest, int indent = 2) :
		myDest(dest),
		myIsAtStartOfLine(true),
		myIndent(indent, ' '),
		myOwner(NULL)
	{
	}
	explicit IndentingOStreambuf(std::ostream& dest, int indent = 2) :
		myDest(dest.rdbuf()),
		myIsAtStartOfLine(true),
		myIndent(indent, ' '),
		myOwner(&dest)
	{
		myOwner->rdbuf(this);
	}
	virtual ~IndentingOStreambuf()
	{
		if (myOwner != NULL) {
			myOwner->rdbuf(myDest);
		}
	}

protected:
	virtual int overflow(int ch)
	{
		if (myIsAtStartOfLine && ch != '\n') {
			myDest->sputn( myIndent.data(), myIndent.size() );
		}
		myIsAtStartOfLine = ch == '\n';
		return myDest->sputc(ch);
	}

private:
	std::streambuf* myDest;
	bool myIsAtStartOfLine;
	std::string myIndent;
	std::ostream* myOwner;
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
STREAM & operator << (STREAM& s, const ClassStat& classStat)
{
	s << "+ " << classStat.getName() << std::endl;
	IndentingOStreambuf ts(s);
	for (size_t i = 0; i != classStat.getMethodCount(); ++i) {
		s << classStat.getMethodStat(i) << std::endl;
	}
	for (ClassStat::ClassStatConstIterator it = classStat.getClass_begin(); it != classStat.getClass_end(); ++it) {
		s << *it->second;
	}
	return s;
}

template <typename STREAM>
STREAM & operator << (STREAM& s, const NamespaceStat& namespaceStat)
{
	s << "* " << namespaceStat.getName() << std::endl;
	//s << fileStat.getStat() << std::endl;

	IndentingOStreambuf ts(s);
	for (unsigned int i = 0; i != namespaceStat.getFunctionCount(); ++i) {
		const FuncStat& funcStat = namespaceStat.getFuncStat(i);
		s << funcStat << std::endl;
	}
	for (NamespaceStat::ClassStatConstIterator it = namespaceStat.getClass_begin(); it != namespaceStat.getClass_end(); ++it) {
		s << *it->second;
	}

	for (NamespaceStat::NamespaceStatConstIterator it = namespaceStat.getNamespace_begin(); it != namespaceStat.getNamespace_end(); ++it) {
		s << *it->second;
	}
	return s;
}

template <typename STREAM>
STREAM & operator << (STREAM& s, const FileStat& fileStat)
{
	s << fileStat.getFilename() << std::endl;
	//s << fileStat.getStat() << std::endl;

	{
		IndentingOStreambuf ts(s);
		for (unsigned int i = 0; i != fileStat.getFunctionCount(); ++i) {
			const FuncStat& funcStat = fileStat.getFuncStat(i);
			s << funcStat << std::endl;
		}
	}
	for (FileStat::NamespaceStatConstIterator it = fileStat.getNamespace_begin(); it != fileStat.getNamespace_end(); ++it) {
		s << *it->second;
	}
	for (FileStat::ClassStatConstIterator it = fileStat.getClass_begin(); it != fileStat.getClass_end(); ++it) {
		s << *it->second;
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

