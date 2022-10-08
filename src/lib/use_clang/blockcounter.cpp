/*
** Copyright 2013-2022 Joris Dauphin
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

#include "blockcounter.h"

#include "../filestat.h"
#include "utils.h"

#include <clang-c/Index.h>

#include <cassert>
#include <optional>

namespace ccccc::use_clang
{

namespace
{
enum class IfStatus
{
	InCondition,
	InIfBranch,
	InElseBranch
};

class ClientData
{
public:
	ClientData(const char* filename, std::optional<IfStatus> ifStatus) :
		m_filename(filename),
		m_ifStatus(ifStatus)
	{}

	const char* getFilename() const { return m_filename; }
	unsigned int getNestedCount() const { return m_nestedCount; }
	const std::optional<IfStatus>& getIfStatus() const { return m_ifStatus; }
	void nextIfStatus()
	{
		assert(m_ifStatus);
		switch (*m_ifStatus) {
			case IfStatus::InCondition: m_ifStatus = IfStatus::InIfBranch; break;
			case IfStatus::InIfBranch: m_ifStatus = IfStatus::InElseBranch; break;
			default: assert(false);
		}
	}

	void updateNestedCount(unsigned int nestedCount)
	{
		m_nestedCount = std::max(m_nestedCount, nestedCount);
	}

private:
	const char* m_filename;
	unsigned int m_nestedCount = 0;
	std::optional<IfStatus> m_ifStatus;
};

enum CXChildVisitResult BlockCounterVisitor(CXCursor cursor, CXCursor parent, CXClientData user_data)
{
	ClientData* client_data = reinterpret_cast<ClientData*>(user_data);
	const char* filename = client_data->getFilename();

	if (isInFile(filename, cursor) == false) {
		return CXChildVisit_Continue;
	}
	// special treatment to not increase block count for `else if`
	if (client_data->getIfStatus()) {
		const unsigned int childNestedBlockCount = BlockCounter::ComputeNestedBlockCount(filename, cursor);

		if (*client_data->getIfStatus() == IfStatus::InElseBranch) {
			if (clang_getCursorKind(cursor) == CXCursor_IfStmt) {
				client_data->updateNestedCount(childNestedBlockCount);
			} else {
				client_data->updateNestedCount(1 + childNestedBlockCount);
			}
		} else {
			client_data->updateNestedCount(childNestedBlockCount + 1);
			client_data->nextIfStatus();
		}
		return CXChildVisit_Continue;
	}
	if (clang_getCursorKind(cursor) == CXCursor_IfStmt) {
		unsigned int childNestedBlockCount = BlockCounter::ComputeNestedBlockCount(filename, cursor);
		client_data->updateNestedCount(childNestedBlockCount);
		return CXChildVisit_Continue;
	}
	if (clang_getCursorKind(cursor) == CXCursor_CompoundStmt) {
		const bool parentMightUseBlock =
			(clang_getCursorKind(parent) == CXCursor_IfStmt
		     || clang_getCursorKind(parent) == CXCursor_ForStmt
		     || clang_getCursorKind(parent) == CXCursor_WhileStmt
		     || clang_getCursorKind(parent) == CXCursor_DoStmt
		     || clang_getCursorKind(parent) == CXCursor_SwitchStmt);
		const unsigned int childNestedBlockCount = BlockCounter::ComputeNestedBlockCount(filename, cursor);

		client_data->updateNestedCount(childNestedBlockCount + (parentMightUseBlock ? 0 : 1));
		return CXChildVisit_Continue;
	}
	if (clang_getCursorKind(cursor) == CXCursor_ForStmt
	    || clang_getCursorKind(cursor) == CXCursor_WhileStmt
	    || clang_getCursorKind(cursor) == CXCursor_DoStmt
	    || clang_getCursorKind(cursor) == CXCursor_SwitchStmt) {
		unsigned int childNestedBlockCount = BlockCounter::ComputeNestedBlockCount(filename, cursor);
		client_data->updateNestedCount(1 + childNestedBlockCount);
		return CXChildVisit_Continue;
	}
	return CXChildVisit_Recurse;
}

} // namespace

unsigned int BlockCounter::ComputeNestedBlockCount(const char* filename, const CXCursor& cursor)
{
	ClientData clientData(filename,
	                      clang_getCursorKind(cursor) == CXCursor_IfStmt ? std::optional{IfStatus::InCondition} : std::nullopt);
	clang_visitChildren(cursor, BlockCounterVisitor, &clientData);
	return clientData.getNestedCount();
}

} // namespace ccccc::use_clang
