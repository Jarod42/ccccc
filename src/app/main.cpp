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

#include <iostream>

#include "cccc_clang_api.h"
#include "utils.h"

#include <ctemplate/template.h>

void feedDict(const ccccc::FuncStat& funcStat, const std::string& namespacesName, const std::string& classesName, ctemplate::TemplateDictionary* dict)
{
	ctemplate::TemplateDictionary& sectionDict = *dict->AddSectionDictionary("ForEachFunctions");

	sectionDict.SetValue("funcName", funcStat.getName());
	sectionDict.SetIntValue("lineDefinition", funcStat.getLineDefinition());
	sectionDict.SetIntValue("LOCphy", funcStat.getLineCount().getLineOfCode_physic());
	sectionDict.SetIntValue("LOCpro", funcStat.getLineCount().getLineOfCode_program());
	sectionDict.SetIntValue("LOCcom", funcStat.getLineCount().getLineOfCode_comment());
	sectionDict.SetIntValue("LOCbl", funcStat.getLineCount().getLineOfCode_blank());
	sectionDict.SetIntValue("MVG", funcStat.getMcCabeCyclomaticNumber());

	sectionDict.SetValue("namespacesName", namespacesName);
	sectionDict.SetValue("classesName", classesName);
}

void feedDict(const ccccc::ClassStat& classStat, const std::string& namespacesName, std::string classesName, ctemplate::TemplateDictionary* dict)
{
	if (classesName.empty() == false) {
		classesName += "::";
	}
	classesName += classStat.getName();

	for (size_t i = 0; i != classStat.getMethodCount(); ++i) {
		feedDict(classStat.getMethodStat(i), namespacesName, classesName, dict);
	}
	for (ccccc::ClassStat::ClassStatConstIterator it = classStat.getClass_begin(); it != classStat.getClass_end(); ++it) {
		feedDict(*it->second, namespacesName, classesName, dict);
	}
}

void feedDict(const ccccc::NamespaceStat& namespaceStat, std::string namespacesName, ctemplate::TemplateDictionary* dict)
{
	if (namespacesName.empty() == false) {
		namespacesName += "::";
	}
	if (namespaceStat.getName().empty()) {
		namespacesName += "{anonymous}";
	} else {
		namespacesName += namespaceStat.getName();
	}
	for (unsigned int i = 0; i != namespaceStat.getFunctionCount(); ++i) {
		const ccccc::FuncStat& funcStat = namespaceStat.getFuncStat(i);
		feedDict(funcStat, namespacesName, "", dict);
	}
	for (ccccc::NamespaceStat::ClassStatConstIterator it = namespaceStat.getClass_begin(); it != namespaceStat.getClass_end(); ++it) {
		feedDict(*it->second, namespacesName, "", dict);
	}
	for (ccccc::NamespaceStat::NamespaceStatConstIterator it = namespaceStat.getNamespace_begin(); it != namespaceStat.getNamespace_end(); ++it) {
		feedDict(*it->second, namespacesName, dict);
	}
}

void feedDict(const ccccc::FileStat& fileStat, ctemplate::TemplateDictionary* dict)
{
	ctemplate::TemplateDictionary* sectionDict = dict->AddSectionDictionary("ForEachFiles");

	sectionDict->SetValue("filename", fileStat.getFilename());
	for (unsigned int i = 0; i != fileStat.getFunctionCount(); ++i) {
		feedDict(fileStat.getFuncStat(i), "", "", sectionDict);
	}
	for (ccccc::FileStat::NamespaceStatConstIterator it = fileStat.getNamespace_begin(); it != fileStat.getNamespace_end(); ++it) {
		feedDict(*it->second, "", sectionDict);
	}
	for (ccccc::FileStat::ClassStatConstIterator it = fileStat.getClass_begin(); it != fileStat.getClass_end(); ++it) {
		feedDict(*it->second, "", "", sectionDict);
	}
}

std::string normalizePath(const std::string& path)
{
	std::string res = path;

	for (std::string::size_type t = res.find("\\"); t != std::string::npos; t = res.find("\\", t)) {
		res[t] = '/';
	}
	return res;
}

// Default bin location is Root/bin/$IDE/$Config
std::string getRootPath(const std::string& exePath)
{
	std::string::size_type t = exePath.rfind("/");
	for (int i = 0; i != 2; ++i) {
		if (t == std::string::npos) {
			return exePath;
		}
		t = exePath.rfind("/", t - 1);
	}
	if (exePath.compare(t + 1, 4, "bin/") != 0) {
		return exePath;
	}
	std::string res = exePath.substr(0, t);
	return res;
}

int main(int argc, char* argv[])
{
	ccccc::Parameters params;

	params.Parse(argc, argv);
	ccccc::AllStat allStat;

	allStat.Compute(params);

	const std::string cccccPath = normalizePath(getExePath());
	const std::string cccccRoot = getRootPath(cccccPath);
	ctemplate::TemplateDictionary dict("root");
	const std::string templateFilename = cccccRoot + "/template/html/template.tpl";

	dict.SetFilename(templateFilename);
	dict.SetValue("cccccPath", cccccPath);
	dict.SetValue("cccccRoot", cccccRoot);

	for (unsigned int i = 0; i != allStat.getFileCount(); ++i) {
		const ccccc::FileStat& filestat = allStat.getFileStat(i);

		feedDict(filestat, &dict);
	}

	std::string output;
	ctemplate::ExpandTemplate(templateFilename, ctemplate::DO_NOT_STRIP, &dict, &output);
	std::cout << output;
	return 0;
}
