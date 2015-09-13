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

	for (const auto& includePath : param.IncludePaths()) {
		args->push_back("-I");
		args->push_back(includePath.c_str());
	}

	for (const auto& define : param.Defines()) {
		args->push_back("-D");
		args->push_back(define.c_str());
	}

	for (const auto& extra : param.Extras()) {
		args->push_back(extra.c_str());
	}
}

AllStat::~AllStat()
{
}

void AllStat::Compute(const Parameters& param)
{
	const int excludeDeclsFromPCH = 1;
	const int displayDiagnostics = 1;
	CXIndex index = clang_createIndex(excludeDeclsFromPCH, displayDiagnostics);

	std::vector<const char*> args;

	GetClangParamFromParam(param, &args);
	for (const std::string& filename : param.Filenames()) {
		CXTranslationUnit tu = clang_parseTranslationUnit(index, filename.c_str(), &args[0], args.size(), 0, 0, 0);

		if (tu && use_clang::isValid(tu)) {
			auto fileStat = std::make_unique<FileStat>(filename);
			use_clang::FileStatTool::Compute(tu, fileStat.get());
			m_filesStat.push_back(std::move(fileStat));
		} else {
			// some errors (file not found)
		}
		clang_disposeTranslationUnit(tu);
	}
	clang_disposeIndex(index);
}

} // namespace ccccc
