/*
** Copyright 2012-2015 Joris Dauphin
*/
/*
**  This file is part of CCCCC.
**
**  CCCCC is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  CCCCC is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with CCCCC. If not, see <http://www.gnu.org/licenses/>.
*/

#include "filestattool.h"

#include "../classstat.h"
#include "funcstattool.h"
#include "linecounter.h"
#include "localstattool.h"

#include "utils.h"
#include <vector>

namespace ccccc
{
namespace use_clang
{

namespace
{

class FileStatFeeder
{
public:
	explicit FileStatFeeder(const CXCursor& cursor) : m_lineCounter(cursor)
	{
	}

	void operator()(const CXTranslationUnit& tu, const CXCursor& cursor, const CXToken& token)
	{
		m_lineCounter(tu, cursor, token);
	}

public:
	LineCounter m_lineCounter;
};


class ClientData
{
public:
	explicit ClientData(CXTranslationUnit tu, GlobalData& globalData, FileStat* stat) :
		m_stat(stat),
		m_tu(tu),
		m_globalData(globalData)
	{}

	FileStat& getFileStat() { return *m_stat; }
	const char* getFilename() const { return m_stat->getFilename().c_str(); }
	CXTranslationUnit getCXTranslationUnit() { return m_tu; }
	GlobalData& getGlobalData() { return m_globalData; }

	void PushNamespace(const std::string& name) { namespaceNames.push_back(name);}
	void PopNamespace() { namespaceNames.pop_back(); }
	const std::vector<std::string>& GetNamespaceNames() const { return namespaceNames; }
private:
	FileStat* m_stat;
	CXTranslationUnit m_tu;
	GlobalData& m_globalData;
	std::vector<std::string> namespaceNames;
};

bool IsAKindOfClass(CXCursor cursor)
{
	return clang_getCursorKind(cursor) == CXCursor_StructDecl
		   || clang_getCursorKind(cursor) == CXCursor_ClassDecl
		   || clang_getCursorKind(cursor) == CXCursor_ClassTemplate
		   || clang_getCursorKind(cursor) == CXCursor_ClassTemplatePartialSpecialization;
}


void getParentClasses(CXCursor cursor, std::vector<std::string>* parentClasses)
{
	for (CXCursor parent = clang_getCursorSemanticParent(cursor); IsAKindOfClass(parent); parent = clang_getCursorSemanticParent(parent)) {
		std::string parentStr = getStringAndDispose(clang_getCursorDisplayName(parent));
		(*parentClasses).insert(parentClasses->begin(), parentStr);
	}
}


}

enum CXChildVisitResult
FileStatTool::FileCursorVisitor(CXCursor cursor,
								CXCursor /*parent*/,
								CXClientData user_data)
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

			FuncStatTool::Compute(client_data->getFilename(), client_data->getCXTranslationUnit(), cursor, client_data->getGlobalData(), funcStat);
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

			FuncStatTool::Compute(client_data->getFilename(), client_data->getCXTranslationUnit(), cursor, client_data->getGlobalData(), funcStat);
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

void FileStatTool::Compute(const CXTranslationUnit& tu, GlobalData& globalData, FileStat* stat)
{
	CXCursor cursor = clang_getTranslationUnitCursor(tu);
	FileStatFeeder fileStatFeeder(cursor);

	processTokens(tu, cursor, fileStatFeeder);

	stat->m_lineCount.lineOfCode_physic = fileStatFeeder.m_lineCounter.getLineOfCode_physic();
	stat->m_lineCount.lineOfCode_comment = fileStatFeeder.m_lineCounter.getLineOfCode_comment();
	stat->m_lineCount.lineOfCode_program = fileStatFeeder.m_lineCounter.getLineOfCode_program();
	stat->m_lineCount.lineOfCode_blank = fileStatFeeder.m_lineCounter.getLineOfCode_blank();

	ClientData clientData(tu, globalData, stat);
	clang_visitChildren(cursor, FileCursorVisitor, &clientData);
}

void FileStatTool::PostFeed(const GlobalData& globalData, NamespaceStat* stat)
{
	for (auto& funcStat : stat->m_funcStats) {
		FuncStatTool::PostFeed(globalData, funcStat.get());
	}
	for (auto& p: stat->m_namespaces) {
		FileStatTool::PostFeed(globalData, p.second.get());
	}
	for (auto& p: stat->m_classes) {
		FileStatTool::PostFeed(globalData, p.second.get());
	}
}

void FileStatTool::PostFeed(const GlobalData& globalData, ClassStat* stat)
{
	for (auto& funcStat : stat->m_methodStats) {
		FuncStatTool::PostFeed(globalData, funcStat.get());
	}
}

void FileStatTool::PostFeed(const GlobalData& globalData, FileStat* stat)
{
	PostFeed(globalData, &stat->m_root);
}

} // namespace use_clang
} // namespace ccccc
