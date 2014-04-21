/*
** Copyright 2013-2014 Joris Dauphin
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

#include <clang-c/Index.h>

#include "../filestat.h"
#include "utils.h"

namespace ccccc
{
namespace use_clang
{

namespace
{
class ClientData
{
public:
	explicit ClientData(const char* filename) :
		m_filename(filename),
		m_nestedCount(0)
	{}

	const char* getFilename() const { return m_filename; }
	unsigned int getNestedCount() const { return m_nestedCount; }
	void setNestedCount(unsigned int nestedCount) { m_nestedCount = nestedCount; }
private:
	const char* m_filename;
	unsigned int m_nestedCount;
};

enum CXChildVisitResult BlockCounterVisitor(CXCursor cursor, CXCursor parent, CXClientData user_data)
{
	ClientData* client_data = reinterpret_cast<ClientData*>(user_data);
	const char* filename = client_data->getFilename();

	if (isInFile(filename, cursor) == false) {
		return CXChildVisit_Continue;
	}
	if (clang_getCursorKind(parent) == CXCursor_IfStmt
		&& clang_getCursorKind(cursor) == CXCursor_IfStmt) { // try to manage 'else if' : create bug with if() if() :-/
		unsigned int childNestedBlockCount = BlockCounter::ComputeNestedBlockCount(filename, cursor);
		client_data->setNestedCount(childNestedBlockCount);
		return CXChildVisit_Continue;
	}
	if (clang_getCursorKind(parent) == CXCursor_IfStmt
		|| clang_getCursorKind(parent) == CXCursor_ForStmt
		|| clang_getCursorKind(parent) == CXCursor_WhileStmt
		|| clang_getCursorKind(parent) == CXCursor_DoStmt
		|| clang_getCursorKind(parent) == CXCursor_SwitchStmt
		|| clang_getCursorKind(cursor) == CXCursor_CompoundStmt) {
		unsigned int childNestedBlockCount = BlockCounter::ComputeNestedBlockCount(filename, cursor);
		client_data->setNestedCount(1 + childNestedBlockCount);
		return CXChildVisit_Continue;
	}
	return CXChildVisit_Recurse;
}

}

unsigned int BlockCounter::ComputeNestedBlockCount(const char* filename, const CXCursor& cursor)
{
	ClientData clientData(filename);
	clang_visitChildren(cursor, BlockCounterVisitor, &clientData);
	return clientData.getNestedCount();
}

} // namespace use_clang
} // namespace ccccc
