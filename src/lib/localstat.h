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

#ifndef LOCAL_STAT_H
#define LOCAL_STAT_H

namespace ccccc
{
namespace use_clang
{
	class LocalStatTool;
}
class LocalStat
{
	friend class use_clang::LocalStatTool;
public:
	unsigned int getLineOfCode_physic() const { return lineOfCode_physic; }
	unsigned int getLineOfCode_program() const { return lineOfCode_program; }
	unsigned int getLineOfCode_blank() const { return lineOfCode_blank; }
	unsigned int getLineOfCode_comment() const { return lineOfCode_comment; }
	unsigned int getMcCabeCyclomaticNumber() const { return mcCabeCyclomaticNumber; }

	//private:
	LocalStat();
	LocalStat(unsigned int lineOfCode_physic,
			  unsigned int lineOfCode_program,
			  unsigned int lineOfCode_blank,
			  unsigned int lineOfCode_comment,
			  unsigned int mcCabeCyclomaticNumber);

private:
	unsigned int lineOfCode_physic;      // LOCphy
	unsigned int lineOfCode_program;     // LOCpro
	unsigned int lineOfCode_blank;       // LOCbl
	unsigned int lineOfCode_comment;     // LOCcom
	unsigned int mcCabeCyclomaticNumber; // MVG
};

} // namespace ccccc

#endif // LOCAL_STAT_H
