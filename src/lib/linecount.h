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

#ifndef LINECOUNT_H
#define LINECOUNT_H

#include <cstddef>

namespace ccccc
{
namespace use_clang
{
class FileStatTool;
class FuncStatTool;
} // namespace use_clang

class LineCount
{
	friend class use_clang::FileStatTool;
	friend class use_clang::FuncStatTool;

public:
	std::size_t getLineOfCode_physic() const { return lineOfCode_physic; }
	std::size_t getLineOfCode_program() const { return lineOfCode_program; }
	std::size_t getLineOfCode_blank() const { return lineOfCode_blank; }
	std::size_t getLineOfCode_comment() const { return lineOfCode_comment; }

	//private:
	LineCount() = default;
	LineCount(std::size_t lineOfCode_physic,
	          std::size_t lineOfCode_program,
	          std::size_t lineOfCode_blank,
	          std::size_t lineOfCode_comment);

private:
	std::size_t lineOfCode_physic = 0; // LOCphy
	std::size_t lineOfCode_program = 0; // LOCpro
	std::size_t lineOfCode_blank = 0; // LOCbl
	std::size_t lineOfCode_comment = 0; // LOCcom
};

} // namespace ccccc

#endif // LINECOUNT_H
