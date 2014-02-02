/*
** Copyright 2012 Joris Dauphin
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
#include "parameters.h"
#include "use_clang/filestattool.h"
#include "use_clang/utils.h"
#include <clang-c/Index.h>
#include <vector>
#include <assert.h>

namespace ccccc
{

static void GetClangParamFromParam(const Parameters& param, std::vector<const char* >* args)
{
	assert(args != nullptr);

	for (Parameters::IncludePathConstIterator it = param.IncludePaths_begin(); it != param.IncludePaths_end(); ++it) {
		args->push_back("-I");
		args->push_back(it->c_str());
	}

	for (Parameters::DefineConstIterator it = param.Defines_begin(); it != param.Defines_end(); ++it) {
		args->push_back("-D");
		args->push_back(it->c_str());
	}

	for (Parameters::ExtraConstIterator it = param.Extras_begin(); it != param.Extras_end(); ++it) {
		args->push_back(it->c_str());
	}
}


AllStat::~AllStat()
{
	for (size_t i = 0; i != m_filesStat.size(); ++i) {
		delete m_filesStat[i];
	}
}

void AllStat::Compute(const Parameters& param)
{
	const int excludeDeclsFromPCH = 1;
	const int displayDiagnostics = 1;
	CXIndex index = clang_createIndex(excludeDeclsFromPCH, displayDiagnostics);

	std::vector<const char*> args;

	GetClangParamFromParam(param, &args);
	for (Parameters::FilenameConstIterator it = param.Files_begin(); it != param.Files_end(); ++it) {
		const std::string& filename = *it;
		CXTranslationUnit tu = clang_parseTranslationUnit(index, filename.c_str(), &args[0], args.size(), 0, 0, 0);

		if (tu && use_clang::isValid(tu)) {
			FileStat* fileStat = new FileStat(filename);
			m_filesStat.push_back(fileStat);
			use_clang::FileStatTool::Compute(tu, fileStat);
		} else {
			// some errors (file not found)
		}
		clang_disposeTranslationUnit(tu);
	}
	clang_disposeIndex(index);
}

} // namespace ccccc
