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
	const auto& lineCount = funcStat.getLineCount();
	const auto& halsteadMetric = funcStat.getHalsteadMetric();
	const auto& maintainabilityIndex = funcStat.getMaintainabilityIndex();
	mstch::map sectionDict{{"funcName", funcStat.getName()},
	                       {"namespacesName", namespacesName},
	                       {"classesName", classesName},
	                       {"lineDefinition", static_cast<int>(funcStat.getLineDefinition())},
	                       {"LOCphy", static_cast<int>(lineCount.getLineOfCode_physic())},
	                       {"LOCpro", static_cast<int>(lineCount.getLineOfCode_program())},
	                       {"LOCcom", static_cast<int>(lineCount.getLineOfCode_comment())},
	                       {"LOCbl", static_cast<int>(lineCount.getLineOfCode_blank())},
	                       {"MVG", static_cast<int>(funcStat.getMcCabeCyclomaticNumber())},
	                       {"CallCount", static_cast<int>(funcStat.getCallCount())},
	                       {"CallerCount", static_cast<int>(funcStat.getCallerCount())},
	                       {"Halstead_n", static_cast<int>(halsteadMetric.getVocabularySize())},
	                       {"Halstead_N", static_cast<int>(halsteadMetric.getProgramLength())},
	                       {"Halstead_V", halsteadMetric.getVolume()},
	                       {"Halstead_D", halsteadMetric.getDifficulty()},
	                       {"Halstead_E", halsteadMetric.getEffort()},
	                       {"Halstead_B", halsteadMetric.getDeliveredBugCount()},
	                       {"Halstead_T", halsteadMetric.getTimeToImplement()},
	                       {"MIwoc", maintainabilityIndex.getMaintainabilityIndexWithoutComments()},
	                       {"MIcw", maintainabilityIndex.getMaintainabilityIndexCommentWeight()},
	                       {"MI", maintainabilityIndex.getMaintainabilityIndex()},
	                       {"NestedBlockCount", static_cast<int>(funcStat.getNestedBlockCount())}};

	if (funcStat.isExplicit()) {
		sectionDict["explicit"] = true;
	}
	if (funcStat.isOverridden()) {
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
	mstch::map sectionDict{
		{"filename", std::filesystem::relative(fileStat.getFilename(), root).string()},
	};
	sectionDict["ForEachFunctions"] = std::move(forEachFunctions);
	return sectionDict;
}
