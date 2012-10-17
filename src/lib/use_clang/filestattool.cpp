

#include "filestattool.h"

#include "funcstattool.h"
#include "localstattool.h"

#include "utils.h"
#include <vector>

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

	void PushNamespace(const std::string &name) { namespaceNames.push_back(name);}
	void PopNamespace() { namespaceNames.pop_back(); }
	const std::vector<std::string>& GetNamespaceNames() const { return namespaceNames; }
private:
	FileStat* m_stat;
	CXTranslationUnit m_tu;
	std::vector<std::string> namespaceNames;
};

}

enum CXChildVisitResult FileStatTool::FileCursorVisitor(CXCursor cursor, CXCursor parent, CXClientData user_data)
{
	ClientData* client_data = reinterpret_cast<ClientData*>(user_data);
	if (isInFile(client_data->getFilename(), cursor) == false) {
		return CXChildVisit_Continue;
	}

	if (clang_getCursorKind(cursor) == CXCursor_Namespace) {
		VisitNamespace(cursor, client_data);
		return CXChildVisit_Continue;
	} else if (clang_isCursorDefinition(cursor)) {
		if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl
			|| clang_getCursorKind(cursor) == CXCursor_FunctionTemplate) {
			FuncStat* funcStat = client_data->getFileStat().AddFuncStat(getStringAndDispose(clang_getCursorDisplayName(cursor)), client_data->GetNamespaceNames());

			FuncStatTool::Compute(client_data->getCXTranslationUnit(), cursor, funcStat);
			return CXChildVisit_Continue;
		}
	}
	// TODO: namespace, class, template (specialization)...
	return CXChildVisit_Recurse;
}

void FileStatTool::VisitNamespace(CXCursor cursor, CXClientData user_data)
{
	ClientData* client_data = reinterpret_cast<ClientData*>(user_data);
	std::string namespaceName = getStringAndDispose(clang_getCursorDisplayName(cursor));
	client_data->PushNamespace(namespaceName);
	clang_visitChildren(cursor, FileStatTool::FileCursorVisitor, client_data);
	client_data->PopNamespace();
}

void FileStatTool::Compute(const CXTranslationUnit& tu, FileStat* stat)
{
	CXCursor cursor = clang_getTranslationUnitCursor(tu);

	LocalStatTool::Compute(tu, cursor, &stat->m_stat);

	ClientData clientData(tu, stat);
	clang_visitChildren(cursor, FileCursorVisitor, &clientData);
}
