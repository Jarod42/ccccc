
#ifndef LOCAL_STAT_H
#define LOCAL_STAT_H

#include <clang-c/Index.h>

class LocalStat
{
public:
	LocalStat() : lineOfCode_program(0), lineOfCode_comment(0), lineOfCode_blank(0), lineOfCode_physic(0), mcCabeCyclomaticNumber(1) {}

	void Compute(const CXTranslationUnit& tu, const CXCursor& cursor);

	unsigned int getLineOfCode_program() const { return lineOfCode_program; }
	unsigned int getLineOfCode_comment() const { return lineOfCode_comment; }
	unsigned int getLineOfCode_blank() const { return lineOfCode_blank; }
	unsigned int getLineOfCode_physic() const { return lineOfCode_physic; }
	unsigned int getMcCabeCyclomaticNumber() const { return mcCabeCyclomaticNumber; }

private:
	void UpdateMcCabeCyclomaticNumber(const CXTranslationUnit& tu, const CXToken& token);

private:
	unsigned int lineOfCode_program;     // LOCpro
	unsigned int lineOfCode_comment;     // LOCcom
	unsigned int lineOfCode_blank;       // LOCbl
	unsigned int lineOfCode_physic;      // LOCphy
	unsigned int mcCabeCyclomaticNumber; // MVG
};

#endif // LOCAL_STAT_H
