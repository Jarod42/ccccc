/*
** Copyright 2012-2016 Joris Dauphin
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

#include <llvm/Support/CommandLine.h>

#include <iostream>

namespace ccccc
{

	namespace
	{
		void ShowVersion()
		{
			std::cerr << "CCCCC version 1.3" << std::endl;
		}
	}

void Parameters::Parse(const std::string& cccccRoot, int argc, char** argv)
{
	llvm::cl::list<std::string> defines{
		"define",
		llvm::cl::desc("Specify define"),
		llvm::cl::value_desc("define")
	};
	llvm::cl::alias defineAlias{
		"D",
		llvm::cl::desc("Alias for -define"),
		llvm::cl::aliasopt(defines)
	};

	llvm::cl::list<std::string> extraOptions{
		"extra-option",
		llvm::cl::desc("Extra option directly given to the clang parser"),
		llvm::cl::value_desc("extra-option")
	};
	llvm::cl::alias extraAlias{
		"e",
		llvm::cl::desc("Alias for -extra-option"),
		llvm::cl::aliasopt(extraOptions)
	};
	llvm::cl::list<std::string> includes{
		"include-dir",
		llvm::cl::desc("Specify include path"),
		llvm::cl::value_desc("path")
	};
	llvm::cl::alias includeAlias{
		"I",
		llvm::cl::desc("Alias for -include-dir"),
		llvm::cl::aliasopt(includes)
	};
	llvm::cl::opt<std::string> templateFile{
		"template-file",
		llvm::cl::desc("template file to use for the report (default is template/html/template.tpl)"),
		llvm::cl::value_desc("template-file"),
		llvm::cl::init(cccccRoot + "/template/html/template.tpl")
	};
	llvm::cl::alias includeTemplate{
		"t",
		llvm::cl::desc("Alias for -template-file"),
		llvm::cl::aliasopt(templateFile)
	};
	llvm::cl::opt<std::string> pch{
		"pch",
		llvm::cl::desc("Compiled header path"),
		llvm::cl::value_desc("pch-file")
	};
	llvm::cl::list<std::string> inputFilenames{
		llvm::cl::Positional,
		llvm::cl::desc("<input files>"),
		llvm::cl::Required
	};
	llvm::cl::SetVersionPrinter(ShowVersion);
	llvm::cl::ParseCommandLineOptions(
		argc,
		argv,
		"Compute metrics from input files and output the report");

	for (const auto& f : inputFilenames) {
		AddFile(f);
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
	SetTemplateFilename(templateFile);
}

}
