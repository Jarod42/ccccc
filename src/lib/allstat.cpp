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

#include "allstat.h"

#include "filestat.h"
#include "globaldata.h"
#include "parameters.h"
#include "use_clang/filestattool.h"
#include "use_clang/utils.h"

#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>

#include <cassert>
#include <iostream>
#include <vector>

namespace ccccc
{
static std::vector<const char*> GetClangParamFromParam(const Parameters& param)
{
	std::vector<const char*> res;

	for (const auto& includePath : param.IncludePaths()) {
		res.push_back("-I");
		res.push_back(includePath.c_str());
	}

	for (const auto& define : param.Defines()) {
		res.push_back("-D");
		res.push_back(define.c_str());
	}

	for (const auto& extra : param.Extras()) {
		res.push_back(extra.c_str());
	}
	return res;
}

static std::vector<const char*> CStringView(const std::vector<std::string>& v)
{
	std::vector<const char*> res;
	res.reserve(v.size());
	for (const auto& s : v) {
		res.emplace_back(s.c_str());
	}
	return res;
}

AllStat::~AllStat()
{
}

std::vector<std::string> GetCompileArgumentsFromDatabase(CXCompilationDatabase compilationDatabase, const std::string& filename)
{
	CXCompileCommands compileCommands = clang_CompilationDatabase_getCompileCommands(compilationDatabase, filename.c_str());
	const unsigned int compileCommandsCount = clang_CompileCommands_getSize(compileCommands);

	if (compileCommandsCount == 0) { // No entries in database
		clang_CompileCommands_dispose(compileCommands);
		return {};
	}
	// Use first one
	CXCompileCommand compileCommand = clang_CompileCommands_getCommand(compileCommands, 0);
	const unsigned int argumentsCount = clang_CompileCommand_getNumArgs(compileCommand);

	std::vector<std::string> res;
	for (unsigned int i = 0; i != argumentsCount; ++i) {
		res.push_back(use_clang::getStringAndDispose(clang_CompileCommand_getArg(compileCommand, i)));
	}
	clang_CompileCommands_dispose(compileCommands);
	return res;
}

void AllStat::Compute(const Parameters& param)
{
	const int excludeDeclsFromPCH = 1;
	const int displayDiagnostics = 1;
	CXIndex index = clang_createIndex(excludeDeclsFromPCH, displayDiagnostics);

	CXCompilationDatabase_Error compilationDatabaseError;
	CXCompilationDatabase compilationDatabase = clang_CompilationDatabase_fromDirectory(".", &compilationDatabaseError);
	std::vector<const char*> argsFromCommandLine = GetClangParamFromParam(param);
	GlobalData globalData;
	for (const std::string& filename : param.Filenames()) {
		auto argsFromDatabase = GetCompileArgumentsFromDatabase(compilationDatabase, filename);
		CXTranslationUnit tu;

		if (argsFromDatabase.empty()) {
			tu = clang_parseTranslationUnit(index, filename.c_str(), argsFromCommandLine.data(), argsFromCommandLine.size(), 0, 0, 0);
		} else {
			auto args = CStringView(argsFromDatabase);
			tu = clang_parseTranslationUnit(index, nullptr, args.data(), args.size(), 0, 0, 0);
		}

		if (tu && use_clang::isValid(tu)) {
			auto fileStat = std::make_unique<FileStat>(filename);
			use_clang::FileStatTool::Compute(tu, globalData, fileStat.get());
			m_filesStat.push_back(std::move(fileStat));
		} else {
			// some errors (file not found)
			std::cerr << "Failed to parse " << filename << std::endl;
		}
		clang_disposeTranslationUnit(tu);
	}
	clang_CompilationDatabase_dispose(compilationDatabase);
	clang_disposeIndex(index);
	for (auto& fileStat : m_filesStat) {
		use_clang::FileStatTool::PostFeed(globalData, fileStat.get());
	}
}

} // namespace ccccc
