

#include "localstat.h"

namespace ccccc
{

LocalStat::LocalStat() :
	lineOfCode_physic(0),
	lineOfCode_program(0),
	lineOfCode_blank(0),
	lineOfCode_comment(0),
	mcCabeCyclomaticNumber(1)
{}

LocalStat::LocalStat(unsigned int lineOfCode_physic,
					 unsigned int lineOfCode_program,
					 unsigned int lineOfCode_blank,
					 unsigned int lineOfCode_comment,
					 unsigned int mcCabeCyclomaticNumber) :
	lineOfCode_physic(lineOfCode_physic),
	lineOfCode_program(lineOfCode_program),
	lineOfCode_blank(lineOfCode_blank),
	lineOfCode_comment(lineOfCode_comment),
	mcCabeCyclomaticNumber(mcCabeCyclomaticNumber)
{}

} // namespace ccccc
