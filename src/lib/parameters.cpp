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

#include "parameters.h"

#include "use_clang/utils.h"

#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>
#include <llvm/Support/CommandLine.h>

#include <filesystem>
#include <iostream>

namespace ccccc
{

namespace
{
void ShowVersion(llvm::raw_ostream& os)
{
	os << "CCCCC version 1.3\n";
	os << "CINDEX version " << CINDEX_VERSION << "\n";
}

void AddFilesFromDatabase(Parameters& parameters, std::filesystem::path compile_commands_json)
{
	CXCompilationDatabase_Error errorCode;
	CXCompilationDatabase db = clang_CompilationDatabase_fromDirectory(
		compile_commands_json.parent_path().string().c_str(), &errorCode);

	if (errorCode != CXCompilationDatabase_NoError) {
		std::cerr << "Cannot open " << compile_commands_json << std::endl;
		clang_CompilationDatabase_dispose(db);
		return;
	}

	CXCompileCommands commands = clang_CompilationDatabase_getAllCompileCommands(db);
	const unsigned size = clang_CompileCommands_getSize(commands);
	for (unsigned int i = 0; i != size; ++i) {
		const CXCompileCommand command = clang_CompileCommands_getCommand(commands, i);
		parameters.AddFile(
			use_clang::getStringAndDispose(clang_CompileCommand_getFilename(command)));
	}

	clang_CompileCommands_dispose(commands);
	clang_CompilationDatabase_dispose(db);
}

} // namespace

void Parameters::Parse(const std::filesystem::path& cccccRoot, int argc, char** argv)
{
	llvm::cl::list<std::string> defines{
		"define", llvm::cl::desc("Specify define"), llvm::cl::value_desc("define")};
	llvm::cl::alias defineAlias{
		"D", llvm::cl::desc("Alias for -define"), llvm::cl::aliasopt(defines)};

	llvm::cl::list<std::string> extraOptions{
		"extra-option",
		llvm::cl::desc("Extra option directly given to the clang parser"),
		llvm::cl::value_desc("extra-option")};
	llvm::cl::alias extraAlias{
		"e", llvm::cl::desc("Alias for -extra-option"), llvm::cl::aliasopt(extraOptions)};
	llvm::cl::list<std::string> includes{
		"include-dir", llvm::cl::desc("Specify include path"), llvm::cl::value_desc("path")};
	llvm::cl::alias includeAlias{
		"I", llvm::cl::desc("Alias for -include-dir"), llvm::cl::aliasopt(includes)};
	llvm::cl::opt<std::string> templateFile{
		"template-file",
		llvm::cl::desc(
			"template file to use for the report (default is template/html/template.tpl)"),
		llvm::cl::value_desc("template-file"),
		llvm::cl::init((cccccRoot / "template/html/template.tpl").string())};
	llvm::cl::alias includeTemplate{
		"t", llvm::cl::desc("Alias for -template-file"), llvm::cl::aliasopt(templateFile)};
	llvm::cl::opt<std::string> pch{
		"pch", llvm::cl::desc("Compiled header path"), llvm::cl::value_desc("pch-file")};
	llvm::cl::opt<std::string> sourceRoot{
		"source-root",
		llvm::cl::desc("source root directory (filename display would be relative to that path) "
	                   "(default is current working directory)"),
		llvm::cl::value_desc("source-root"),
		llvm::cl::init(std::filesystem::current_path().string())};
	llvm::cl::alias sourceRootAlias{
		"R", llvm::cl::desc("Alias for -source-root"), llvm::cl::aliasopt(sourceRoot)};

	llvm::cl::list<std::string> inputFilenames{
		llvm::cl::Positional, llvm::cl::desc("<input files>"), llvm::cl::OneOrMore};
	llvm::cl::SetVersionPrinter(ShowVersion);
	llvm::cl::ParseCommandLineOptions(
		argc, argv, "Compute metrics from input files and output the report");

	for (const std::filesystem::path f : inputFilenames) {
		if (f.filename() == "compile_commands.json") {
			if (GetDatabaseRoot().empty()) {
				SetDatabaseRoot(f.parent_path());
			}
			AddFilesFromDatabase(*this, f);
		} else {
			AddFile(f);
		}
	}
	for (const auto& d : defines) {
		AddDefine(d);
	}
	for (const auto& i : includes) {
		AddInclude(i);
	}
	for (const auto& e : extraOptions) {
		AddExtra(e);
	}
	if (!pch.empty()) {
		SetPch(pch);
	}
	SetSourceRoot(std::string(sourceRoot));
	SetTemplateFilename(std::string(templateFile));
}

} // namespace ccccc
