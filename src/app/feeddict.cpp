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

#include "cccc_clang_api.h"

#include <ctemplate/template.h>
#include <sstream>

static void SetDoubleValue(ctemplate::TemplateDictionary& dict, const char* key, double value)
{
	std::stringstream ss;

	ss << value;
	dict.SetValue(key, ss.str());
}

void feedDict(const ccccc::FuncStat& funcStat,
              const std::string& namespacesName,
              const std::string& classesName,
              ctemplate::TemplateDictionary* dict)
{
	ctemplate::TemplateDictionary& sectionDict = *dict->AddSectionDictionary("ForEachFunctions");

	sectionDict.SetValue("funcName", funcStat.getName());
	if (funcStat.isOverriden()) {
		sectionDict.ShowSection("override");
	} else if (funcStat.isVirtual()) {
		sectionDict.ShowSection("virtual");
	}
	if (funcStat.isStatic()) {
		sectionDict.ShowSection("static");
	}
	if (funcStat.isConst()) {
		sectionDict.ShowSection("const");
	}
	sectionDict.SetIntValue("lineDefinition", funcStat.getLineDefinition());
	sectionDict.SetIntValue("LOCphy", funcStat.getLineCount().getLineOfCode_physic());
	sectionDict.SetIntValue("LOCpro", funcStat.getLineCount().getLineOfCode_program());
	sectionDict.SetIntValue("LOCcom", funcStat.getLineCount().getLineOfCode_comment());
	sectionDict.SetIntValue("LOCbl", funcStat.getLineCount().getLineOfCode_blank());
	sectionDict.SetIntValue("MVG", funcStat.getMcCabeCyclomaticNumber());

	sectionDict.SetIntValue("CallCount", funcStat.getCallCount());
	sectionDict.SetIntValue("CallerCount", funcStat.getCallerCount());

	sectionDict.SetIntValue("Halstead_n", funcStat.getHalsteadMetric().getVocabularySize());
	sectionDict.SetIntValue("Halstead_N", funcStat.getHalsteadMetric().getProgramLength());
	SetDoubleValue(sectionDict, "Halstead_V", funcStat.getHalsteadMetric().getVolume());
	SetDoubleValue(sectionDict, "Halstead_D", funcStat.getHalsteadMetric().getDifficulty());
	SetDoubleValue(sectionDict, "Halstead_E", funcStat.getHalsteadMetric().getEffort());
	SetDoubleValue(sectionDict, "Halstead_B", funcStat.getHalsteadMetric().getDeliveredBugCount());
	SetDoubleValue(sectionDict, "Halstead_T", funcStat.getHalsteadMetric().getTimeToImplement());

	SetDoubleValue(sectionDict,
	               "MIwoc",
	               funcStat.getMaintainabilityIndex().getMaintainabilityIndexWithoutComments());
	SetDoubleValue(sectionDict,
	               "MIcw",
	               funcStat.getMaintainabilityIndex().getMaintainabilityIndexCommentWeight());
	SetDoubleValue(sectionDict, "MI", funcStat.getMaintainabilityIndex().getMaintainabilityIndex());

	sectionDict.SetIntValue("NestedBlockCount", funcStat.getNestedBlockCount());

	sectionDict.SetValue("namespacesName", namespacesName);
	sectionDict.SetValue("classesName", classesName);
}

void feedDict(const ccccc::ClassStat& classStat,
              const std::string& namespacesName,
              std::string classesName,
              ctemplate::TemplateDictionary* dict)
{
	if (classesName.empty() == false) {
		classesName += "::";
	}
	classesName += classStat.getName();

	for (size_t i = 0; i != classStat.getMethodCount(); ++i) {
		feedDict(classStat.getMethodStat(i), namespacesName, classesName, dict);
	}
	for (const auto& p : classStat.getInnerClasses()) {
		feedDict(*p.second, namespacesName, classesName, dict);
	}
}

void feedDict(const ccccc::NamespaceStat& namespaceStat,
              std::string namespacesName,
              ctemplate::TemplateDictionary* dict)
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
	for (const auto& p : namespaceStat.getClasses()) {
		feedDict(*p.second, namespacesName, "", dict);
	}
	for (const auto& p : namespaceStat.getNamespaces()) {
		feedDict(*p.second, namespacesName, dict);
	}
}

void feedDict(const ccccc::FileStat& fileStat, ctemplate::TemplateDictionary* dict)
{
	ctemplate::TemplateDictionary* sectionDict = dict->AddSectionDictionary("ForEachFiles");

	sectionDict->SetValue("filename", fileStat.getFilename().string());
	for (unsigned int i = 0; i != fileStat.getFunctionCount(); ++i) {
		feedDict(fileStat.getFuncStat(i), "", "", sectionDict);
	}
	for (const auto& p : fileStat.getNamespaces()) {
		feedDict(*p.second, "", sectionDict);
	}
	for (const auto& p : fileStat.getClasses()) {
		feedDict(*p.second, "", "", sectionDict);
	}
}
