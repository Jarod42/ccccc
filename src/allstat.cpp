

#include "allstat.h"

#include "filestat.h"
#include "use_clang/filestattool.h"
#include "use_clang/utils.h"
#include <clang-c/Index.h>

AllStat::~AllStat()
{
	for (size_t i = 0; i != m_filesStat.size(); ++i) {
		delete m_filesStat[i];
	}
}

void AllStat::Compute(const char* filename, int extraArgsCount, const char *extraArgs[])
{
	CXIndex index = clang_createIndex(1, 1);

	// Hard coded system headersHard coded
	#define MINGWPATH "d:/Programs/mingw-4.6.1"
	#define MINGWPATH2 MINGWPATH"/lib/gcc/mingw32/4.6.1"
	const char *defaultArgs[] = {
		"-I"MINGWPATH"/include",
		"-I"MINGWPATH2"/include/c++",
		"-I"MINGWPATH2"/include/c++/mingw32",
		"-I"MINGWPATH2"/include/c++/backward",
		"-I"MINGWPATH2"/include",
		"-I"MINGWPATH2"/include-fixed",
	};
	std::vector<const char*> args(defaultArgs, defaultArgs + ARRAY_SIZE(defaultArgs));
	args.insert(args.end(), extraArgs, extraArgs + extraArgsCount);

	CXTranslationUnit tu = clang_parseTranslationUnit(index, filename, &args[0], args.size(), 0, 0, 0);

	if (tu) {
		FileStat *fileStat = new FileStat(filename);
		m_filesStat.push_back(fileStat);
		FileStatTool::Compute(tu, fileStat);
	}
	clang_disposeTranslationUnit(tu);
	clang_disposeIndex(index);
}
