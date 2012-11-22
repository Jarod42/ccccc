

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
	assert(args != NULL);

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

		if (tu && isValid(tu)) {
			FileStat* fileStat = new FileStat(filename);
			m_filesStat.push_back(fileStat);
			FileStatTool::Compute(tu, fileStat);
		} else {
			// some errors (file not found)

		}
		clang_disposeTranslationUnit(tu);
	}
	clang_disposeIndex(index);
}

}
