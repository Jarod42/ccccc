/*
** Copyright 2012-2022 Joris Dauphin
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

AllStat::~AllStat() = default;

std::pair<std::filesystem::path, std::vector<std::string>>
GetCompileArgumentsFromDatabase(CXCompilationDatabase compilationDatabase,
                                const std::filesystem::path& filename)
{
	CXCompileCommands compileCommands = clang_CompilationDatabase_getCompileCommands(
		compilationDatabase, filename.string().c_str());
	const std::size_t compileCommandsCount = clang_CompileCommands_getSize(compileCommands);

	if (compileCommandsCount == 0) { // No entries in database
		clang_CompileCommands_dispose(compileCommands);
		return {};
	}
	// Use first one
	CXCompileCommand compileCommand = clang_CompileCommands_getCommand(compileCommands, 0);
	std::filesystem::path workingDirectory =
		use_clang::getStringAndDispose(clang_CompileCommand_getDirectory(compileCommand));
	const unsigned int argumentsCount = clang_CompileCommand_getNumArgs(compileCommand);

	std::vector<std::string> arguments;
	for (unsigned int i = 0; i != argumentsCount; ++i) {
		arguments.push_back(
			use_clang::getStringAndDispose(clang_CompileCommand_getArg(compileCommand, i)));
	}
	clang_CompileCommands_dispose(compileCommands);
	return {std::move(workingDirectory), std::move(arguments)};
}

void AllStat::Compute(const Parameters& param)
{
	const int excludeDeclsFromPCH = 1;
	const int displayDiagnostics = 1;
	CXIndex index = clang_createIndex(excludeDeclsFromPCH, displayDiagnostics);

	CXCompilationDatabase_Error compilationDatabaseError;
	CXCompilationDatabase compilationDatabase =
		clang_CompilationDatabase_fromDirectory(".", &compilationDatabaseError);
	const std::vector<const char*> argsFromCommandLine = GetClangParamFromParam(param);
	GlobalData globalData;
	const std::filesystem::path currentWorkingDirectory = std::filesystem::current_path();
	for (const std::filesystem::path& filename : param.Filenames()) {
		const auto absFilename = std::filesystem::absolute(filename);
		std::cerr << "processing: " << absFilename << std::endl;
		const auto [workingDirectory, argsFromDatabase] =
			GetCompileArgumentsFromDatabase(compilationDatabase, absFilename);
		CXTranslationUnit tu;

		if (argsFromDatabase.empty()) {
			tu = clang_parseTranslationUnit(index,
			                                absFilename.string().c_str(),
			                                argsFromCommandLine.data(),
			                                static_cast<int>(argsFromCommandLine.size()),
			                                0,
			                                0,
			                                0);
		} else {
			std::filesystem::current_path(workingDirectory);
			auto args = CStringView(argsFromDatabase);
			tu = clang_parseTranslationUnit(
				index, nullptr, args.data(), static_cast<int>(args.size()), 0, 0, 0);
			std::filesystem::current_path(currentWorkingDirectory);
		}

		if (tu && use_clang::isValid(tu)) {
			auto fileStat = std::make_unique<FileStat>(absFilename);
			use_clang::FileStatTool::Compute(tu, globalData, fileStat.get());
			m_filesStat.push_back(std::move(fileStat));
		} else {
			// some errors (file not found)
			std::cerr << "Failed to parse " << absFilename << std::endl;
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
