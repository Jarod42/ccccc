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

#include "feeddict.h"

#include "cccc_clang_api.h"

#include <sstream>

mstch::map makeDict(const ccccc::FuncStat& funcStat,
                    const std::string& namespacesName,
                    const std::string& classesName)
{
	mstch::map sectionDict; // = *dict->AddSectionDictionary("ForEachFunctions");

	sectionDict["funcName"] = funcStat.getName();
	if (funcStat.isExplicit()) {
		sectionDict["explicit"] = true;
	}
	if (funcStat.isOverriden()) {
		sectionDict["override"] = true;
	} else if (funcStat.isVirtual()) {
		sectionDict["virtual"] = true;
	}
	if (funcStat.isStatic()) {
		sectionDict["static"] = true;
	}
	if (funcStat.isConst()) {
		sectionDict["const"] = true;
	}
	sectionDict["lineDefinition"] = static_cast<int>(funcStat.getLineDefinition());
	sectionDict["LOCphy"] = static_cast<int>(funcStat.getLineCount().getLineOfCode_physic());
	sectionDict["LOCpro"] = static_cast<int>(funcStat.getLineCount().getLineOfCode_program());
	sectionDict["LOCcom"] = static_cast<int>(funcStat.getLineCount().getLineOfCode_comment());
	sectionDict["LOCbl"] = static_cast<int>(funcStat.getLineCount().getLineOfCode_blank());
	sectionDict["MVG"] = static_cast<int>(funcStat.getMcCabeCyclomaticNumber());

	sectionDict["CallCount"] = static_cast<int>(funcStat.getCallCount());
	sectionDict["CallerCount"] = static_cast<int>(funcStat.getCallerCount());

	sectionDict["Halstead_n"] = static_cast<int>(funcStat.getHalsteadMetric().getVocabularySize());
	sectionDict["Halstead_N"] = static_cast<int>(funcStat.getHalsteadMetric().getProgramLength());
	sectionDict["Halstead_V"] = funcStat.getHalsteadMetric().getVolume();
	sectionDict["Halstead_D"] = funcStat.getHalsteadMetric().getDifficulty();
	sectionDict["Halstead_E"] = funcStat.getHalsteadMetric().getEffort();
	sectionDict["Halstead_B"] = funcStat.getHalsteadMetric().getDeliveredBugCount();
	sectionDict["Halstead_T"] = funcStat.getHalsteadMetric().getTimeToImplement();

	sectionDict["MIwoc"] =
		funcStat.getMaintainabilityIndex().getMaintainabilityIndexWithoutComments();
	sectionDict["MIcw"] = funcStat.getMaintainabilityIndex().getMaintainabilityIndexCommentWeight();
	sectionDict["MI"] = funcStat.getMaintainabilityIndex().getMaintainabilityIndex();

	sectionDict["NestedBlockCount"] = static_cast<int>(funcStat.getNestedBlockCount());

	sectionDict["namespacesName"] = namespacesName;
	sectionDict["classesName"] = classesName;

	return sectionDict;
}

void feedDict(const ccccc::ClassStat& classStat,
              const std::string& namespacesName,
              std::string classesName,
              mstch::array* forEachFunctions)
{
	if (classesName.empty() == false) {
		classesName += "::";
	}
	classesName += classStat.getName();

	for (size_t i = 0; i != classStat.getMethodCount(); ++i) {
		forEachFunctions->emplace_back(
			makeDict(classStat.getMethodStat(i), namespacesName, classesName));
	}
	for (const auto& p : classStat.getInnerClasses()) {
		feedDict(*p.second, namespacesName, classesName, forEachFunctions);
	}
}

void feedDict(const ccccc::NamespaceStat& namespaceStat,
              std::string namespacesName,
              mstch::array* forEachFunctions)
{
	if (namespacesName.empty() == false) {
		namespacesName += "::";
	}
	if (namespaceStat.getName().empty()) {
		namespacesName += "{anonymous}";
	} else {
		namespacesName += namespaceStat.getName();
	}

	for (std::size_t i = 0; i != namespaceStat.getFunctionCount(); ++i) {
		const ccccc::FuncStat& funcStat = namespaceStat.getFuncStat(i);
		forEachFunctions->emplace_back(makeDict(funcStat, namespacesName, ""));
	}

	for (const auto& p : namespaceStat.getClasses()) {
		feedDict(*p.second, namespacesName, "", forEachFunctions);
	}
	for (const auto& p : namespaceStat.getNamespaces()) {
		feedDict(*p.second, namespacesName, forEachFunctions);
	}
}

mstch::map makeDict(const ccccc::FileStat& fileStat, const std::filesystem::path& root)
{
	mstch::map sectionDict;

	sectionDict["filename"] = std::filesystem::relative(fileStat.getFilename(), root).string();
	mstch::array forEachFunctions;
	for (std::size_t i = 0; i != fileStat.getFunctionCount(); ++i) {
		forEachFunctions.emplace_back(makeDict(fileStat.getFuncStat(i), "", ""));
	}
	for (const auto& p : fileStat.getNamespaces()) {
		feedDict(*p.second, "", &forEachFunctions);
	}
	for (const auto& p : fileStat.getClasses()) {
		feedDict(*p.second, "", "", &forEachFunctions);
	}
	sectionDict["ForEachFunctions"] = std::move(forEachFunctions);
	return sectionDict;
}
