

#include "cccc_clang_api.h"


#if 0

#include "localstat.h"
#include "use_clang/localstattool.h"
#include "use_clang/utils.h"
#include <clang-c/Index.h>


class ClientData
{
public:
	explicit ClientData(CXTranslationUnit tu, const char* filename, std::vector<LocalStat>& localStats) :
		m_localStats(localStats),
		m_filename(filename),
		m_tu(tu)
	{}

	std::vector<LocalStat>& getLocalStats() { return m_localStats; }
	const char* getFilename() const { return m_filename; }
	CXTranslationUnit getCXTranslationUnit() { return m_tu; }

private:
	std::vector<LocalStat>& m_localStats;
	const char* m_filename;
	CXTranslationUnit m_tu;
};

enum CXChildVisitResult MyCursorVisitor(CXCursor cursor, CXCursor parent, CXClientData user_data)
{
	ClientData* client_data = reinterpret_cast<ClientData*>(user_data);
	if (isInFile(client_data->getFilename(), cursor) == false) {
		return CXChildVisit_Continue;
	}

	if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl
		&& clang_isCursorDefinition(cursor)) {
		client_data->getLocalStats().push_back(LocalStat(/*getStringAndDispose(clang_getCursorDisplayName(cursor))*/));
		LocalStat &localStat = client_data->getLocalStats().back();
		LocalStatTool::Compute(client_data->getCXTranslationUnit(), cursor, &localStat);
	}
	return CXChildVisit_Recurse;
}

void ComputeStats(const char* filename, int extraArgsCount, const char *extraArgs[], std::vector<LocalStat>& /* OUT */ localStats)
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

	if (tu)
	{
		CXCursor cursor = clang_getTranslationUnitCursor(tu);

		localStats.push_back(LocalStat(/*filename*/));
		LocalStat &localStat = localStats.back();
		LocalStatTool::Compute(tu, cursor, &localStat);

		ClientData clientData(tu, filename, localStats);
		clang_visitChildren(cursor, MyCursorVisitor, &clientData);
	}
	clang_disposeTranslationUnit(tu);
	clang_disposeIndex(index);
}

#endif
