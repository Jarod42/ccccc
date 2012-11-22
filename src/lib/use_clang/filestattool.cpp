

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

bool IsAKindOfClass(CXCursor cursor)
{
	return clang_getCursorKind(cursor) == CXCursor_StructDecl
	|| clang_getCursorKind(cursor) == CXCursor_ClassDecl
	|| clang_getCursorKind(cursor) == CXCursor_ClassTemplate
	|| clang_getCursorKind(cursor) == CXCursor_ClassTemplatePartialSpecialization;
}


void getParentClasses(CXCursor cursor, std::vector<std::string> *parentClasses)
{
	for (CXCursor parent = clang_getCursorSemanticParent(cursor); IsAKindOfClass(parent); parent = clang_getCursorSemanticParent(parent)) {
		std::string parentStr = getStringAndDispose(clang_getCursorDisplayName(parent));
		(*parentClasses).insert(parentClasses->begin(), parentStr);
	}
}


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
			std::vector<std::string> parentClasses; // empty
			const std::string cursorStr = getStringAndDispose(clang_getCursorDisplayName(cursor));
			const CXSourceRange range = clang_getCursorExtent(cursor);
			const unsigned int line = getStartLine(range);
			FuncStat* funcStat = client_data->getFileStat().AddFuncStat(client_data->GetNamespaceNames(), parentClasses, cursorStr, line);

			FuncStatTool::Compute(client_data->getCXTranslationUnit(), cursor, funcStat);
			return CXChildVisit_Continue;
		} else if (clang_getCursorKind(cursor) == CXCursor_CXXMethod
		|| clang_getCursorKind(cursor) == CXCursor_Constructor
		|| clang_getCursorKind(cursor) == CXCursor_Destructor
		|| clang_getCursorKind(cursor) == CXCursor_ConversionFunction) {
		//CXCursor_ClassTemplate
		//CXCursor_ClassTemplatePartialSpecialization
			std::vector<std::string> parentClasses;
			getParentClasses(cursor, &parentClasses);

			std::string cursorStr = getStringAndDispose(clang_getCursorDisplayName(cursor));
			const CXSourceRange range = clang_getCursorExtent(cursor);
			const int line = getStartLine(range);
			// TODO: get ClassStat.
			FuncStat* funcStat = client_data->getFileStat().AddFuncStat(client_data->GetNamespaceNames(), parentClasses, cursorStr, line);

			FuncStatTool::Compute(client_data->getCXTranslationUnit(), cursor, funcStat);
			return CXChildVisit_Continue;
		} else {
			return CXChildVisit_Recurse;
		}
	}
	// TODO: class, template (specialization)...
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
