/*
** Copyright 2012-2024 Joris Dauphin
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
#include <string>

namespace ccccc
{

namespace
{
bool is_subpath(const std::filesystem::path& path, const std::filesystem::path& base)
{
	const auto rel = std::filesystem::relative(path, base);
	return !rel.empty() && rel.native()[0] != '.';
}

bool withinDirectories(const std::filesystem::path& filename,
                       const std::vector<std::filesystem::path>& directories)
{
	return std::any_of(
		directories.begin(), directories.end(), [&](const std::filesystem::path& directory) {
			return is_subpath(filename, directory);
		});
}

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

	for (const auto& directory : parameters.GetExcludeDirectories()) {
		std::cerr << "Exclude directory: " << std::filesystem::absolute(directory) << std::endl;
	}

	CXCompileCommands commands = clang_CompilationDatabase_getAllCompileCommands(db);
	const unsigned size = clang_CompileCommands_getSize(commands);
	for (unsigned int i = 0; i != size; ++i) {
		const CXCompileCommand command = clang_CompileCommands_getCommand(commands, i);
		auto filename = use_clang::getStringAndDispose(clang_CompileCommand_getFilename(command));
		if (!withinDirectories(filename, parameters.GetExcludeDirectories())) {
			std::cerr << "Add " << filename << std::endl;
			parameters.AddFile(filename);
		} else {
			std::cerr << "Exclude " << filename << std::endl;
		}
	}

	clang_CompileCommands_dispose(commands);
	clang_CompilationDatabase_dispose(db);
}

} // namespace

void Parameters::Parse(const std::filesystem::path& cccccRoot, int argc, char** argv)
{
	llvm::cl::OptionCategory cccccCategory{"ccccc Options"};

	llvm::cl::list<std::string> defines{"define",
	                                    llvm::cl::desc("Specify define"),
	                                    llvm::cl::value_desc("define"),
	                                    llvm::cl::cat(cccccCategory)};
	llvm::cl::alias defineAlias{
		"D", llvm::cl::desc("Alias for -define"), llvm::cl::aliasopt(defines)};

	llvm::cl::list<std::string> extraOptions{
		"extra-option",
		llvm::cl::desc("Extra option directly given to the clang parser"),
		llvm::cl::value_desc("extra-option"),
		llvm::cl::cat(cccccCategory)};
	llvm::cl::alias extraAlias{
		"e", llvm::cl::desc("Alias for -extra-option"), llvm::cl::aliasopt(extraOptions)};
	llvm::cl::list<std::string> includes{"include-dir",
	                                     llvm::cl::desc("Specify include path"),
	                                     llvm::cl::value_desc("path"),
	                                     llvm::cl::cat(cccccCategory)};
	llvm::cl::alias includeAlias{
		"I", llvm::cl::desc("Alias for -include-dir"), llvm::cl::aliasopt(includes)};
	llvm::cl::opt<std::string> templateFile{
		"template-file",
		llvm::cl::desc(
			"template file to use for the report (default is template/html/template.tpl)"),
		llvm::cl::value_desc("template-file"),
		llvm::cl::cat(cccccCategory),
		llvm::cl::init((cccccRoot / "template/html/template.tpl").string())};
	llvm::cl::alias templateFileAlias{
		"t", llvm::cl::desc("Alias for -template-file"), llvm::cl::aliasopt(templateFile)};
	llvm::cl::opt<std::string> pch{"pch",
	                               llvm::cl::desc("Compiled header path"),
	                               llvm::cl::value_desc("pch-file"),
	                               llvm::cl::cat(cccccCategory)};
	llvm::cl::opt<std::string> sourceRoot{
		"source-root",
		llvm::cl::desc("source root directory (filename display would be relative to that path) "
	                   "(default is current working directory)"),
		llvm::cl::value_desc("source-root"),
		llvm::cl::cat(cccccCategory),
		llvm::cl::init(std::filesystem::current_path().string())};
	llvm::cl::alias sourceRootAlias{
		"R", llvm::cl::desc("Alias for -source-root"), llvm::cl::aliasopt(sourceRoot)};
	llvm::cl::list<std::string> excludeDirectories{
		"exclude-directory",
		llvm::cl::desc("exclude input files from these directories"),
		llvm::cl::value_desc("exclude-directory"),
		llvm::cl::cat(cccccCategory)};
	llvm::cl::list<std::string> inputFilenames{
		llvm::cl::Positional, llvm::cl::desc("<input files>"), llvm::cl::OneOrMore};

	HideUnrelatedOptions(cccccCategory);

	llvm::cl::AddExtraVersionPrinter(ShowVersion);
	llvm::cl::ParseCommandLineOptions(
		argc, argv, "Compute metrics from input files and output the report");

	for (const auto& directory : excludeDirectories) {
		AddExcludeDirectory(directory);
	}

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

	defines.removeArgument();
	includes.removeArgument();
	inputFilenames.removeArgument();
	excludeDirectories.removeArgument();
	extraOptions.removeArgument();
	pch.removeArgument();
	sourceRoot.removeArgument();
	templateFile.removeArgument();
	defineAlias.removeArgument();
	includeAlias.removeArgument();
	extraAlias.removeArgument();
	sourceRootAlias.removeArgument();
	templateFileAlias.removeArgument();
}

} // namespace ccccc
