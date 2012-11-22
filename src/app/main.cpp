
#include <iostream>

#include "cccc_clang_api.h"
#include "utils.h"

#include <ctemplate/template.h>

void feedDict(const FuncStat& funcStat, const std::string& namespacesName, const std::string& classesName, ctemplate::TemplateDictionary* dict)
{
	ctemplate::TemplateDictionary& sectionDict = *dict->AddSectionDictionary("InFunctions");

	sectionDict.SetValue("funcName", funcStat.getName());
	sectionDict.SetIntValue("lineDefinition", funcStat.getLineDefinition());
	sectionDict.SetIntValue("LOCphy", funcStat.getStat().getLineOfCode_physic());
	sectionDict.SetIntValue("LOCpro", funcStat.getStat().getLineOfCode_program());
	sectionDict.SetIntValue("LOCcom", funcStat.getStat().getLineOfCode_comment());
	sectionDict.SetIntValue("LOCbl", funcStat.getStat().getLineOfCode_blank());
	sectionDict.SetIntValue("MVG", funcStat.getStat().getMcCabeCyclomaticNumber());

	sectionDict.SetValue("namespacesName", namespacesName);
	sectionDict.SetValue("classesName", classesName);
}

void feedDict(const ClassStat& classStat, const std::string& namespacesName, std::string classesName, ctemplate::TemplateDictionary* dict)
{
	if (classesName.empty() == false) {
		classesName += "::";
	}
	classesName += classStat.getName();

	for (size_t i = 0; i != classStat.getMethodCount(); ++i) {
		feedDict(classStat.getMethodStat(i), namespacesName, classesName, dict);
	}
	for (ClassStat::ClassStatConstIterator it = classStat.getClass_begin(); it != classStat.getClass_end(); ++it) {
		feedDict(*it->second, namespacesName, classesName, dict);
	}
}

void feedDict(const NamespaceStat& namespaceStat, std::string namespacesName, ctemplate::TemplateDictionary* dict)
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
		const FuncStat& funcStat = namespaceStat.getFuncStat(i);
		feedDict(funcStat, namespacesName, "", dict);
	}
	for (NamespaceStat::ClassStatConstIterator it = namespaceStat.getClass_begin(); it != namespaceStat.getClass_end(); ++it) {
		feedDict(*it->second, namespacesName, "", dict);
	}
	for (NamespaceStat::NamespaceStatConstIterator it = namespaceStat.getNamespace_begin(); it != namespaceStat.getNamespace_end(); ++it) {
		feedDict(*it->second, namespacesName, dict);
	}
}

void feedDict(const FileStat& fileStat, ctemplate::TemplateDictionary* dict)
{
	dict->SetValue("filename", fileStat.getFilename());
	for (unsigned int i = 0; i != fileStat.getFunctionCount(); ++i) {
		feedDict(fileStat.getFuncStat(i), "", "", dict);
	}
	for (FileStat::NamespaceStatConstIterator it = fileStat.getNamespace_begin(); it != fileStat.getNamespace_end(); ++it) {
		feedDict(*it->second, "", dict);
	}
	for (FileStat::ClassStatConstIterator it = fileStat.getClass_begin(); it != fileStat.getClass_end(); ++it) {
		feedDict(*it->second, "", "", dict);
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

	ctemplate::TemplateDictionary dict("root");

	std::string exePath = normalizePath(getExePath());
	dict.SetValue("cccccPath", exePath);
	dict.SetValue("cccccRoot", getRootPath(exePath));
	for (unsigned int i = 0; i != allStat.getFileCount(); ++i) {
		ctemplate::TemplateDictionary* fileDict = dict.AddSectionDictionary("InFiles");
		const FileStat& filestat = allStat.getFileStat(i);

		feedDict(filestat, fileDict);
	}

	std::string output;
	ctemplate::ExpandTemplate("template/html/template.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);
	std::cout << output;
	return 0;
}
