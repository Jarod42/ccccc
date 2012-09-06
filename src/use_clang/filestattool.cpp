

#include "filestattool.h"

#include "funcstattool.h"
#include "localstattool.h"

#include "utils.h"

namespace {

class ClientData
{
public:
	explicit ClientData(CXTranslationUnit tu, FileStat* stat) :
		m_stat(stat),
		m_tu(tu)
	{}

	FileStat& getFileStat() { return *m_stat; }
	const char* getFilename() const { return m_stat->getFilename().c_str(); }
	CXTranslationUnit getCXTranslationUnit() { return m_tu; }

private:
	FileStat* m_stat;
	CXTranslationUnit m_tu;
};

}
enum CXChildVisitResult FileStatTool::FileCursorVisitor(CXCursor cursor, CXCursor parent, CXClientData user_data)
{
	ClientData* client_data = reinterpret_cast<ClientData*>(user_data);
	if (isInFile(client_data->getFilename(), cursor) == false) {
		return CXChildVisit_Continue;
	}

	if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl
		&& clang_isCursorDefinition(cursor)) {
		FuncStat* funcStat = client_data->getFileStat().AddFuncStat(getStringAndDispose(clang_getCursorDisplayName(cursor)));

		FuncStatTool::Compute(client_data->getCXTranslationUnit(), cursor, funcStat);
		return CXChildVisit_Continue;
	}
	// TODO: namespace, class, ...
	return CXChildVisit_Recurse;
}

void FileStatTool::Compute(const CXTranslationUnit& tu, FileStat* stat)
{
	CXCursor cursor = clang_getTranslationUnitCursor(tu);

	LocalStatTool::Compute(tu, cursor, &stat->m_stat);

	ClientData clientData(tu, stat);
	clang_visitChildren(cursor, FileCursorVisitor, &clientData);
}
