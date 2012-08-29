
#ifndef LOCAL_STAT_H
#define LOCAL_STAT_H

#include <clang-c/Index.h>

#include <string>

class LocalStat
{
public:
	explicit LocalStat(const std::string& moduleName);

	void Compute(const CXTranslationUnit& tu, const CXCursor& cursor);

	const std::string& getModuleName() const { return moduleName; }
	unsigned int getLineOfCode_program() const { return lineOfCode_program; }
	unsigned int getLineOfCode_comment() const { return lineOfCode_comment; }
	unsigned int getLineOfCode_blank() const { return lineOfCode_blank; }
	unsigned int getLineOfCode_physic() const { return lineOfCode_physic; }
	unsigned int getMcCabeCyclomaticNumber() const { return mcCabeCyclomaticNumber; }

private:
	void UpdateMcCabeCyclomaticNumber(const CXTranslationUnit& tu, const CXToken& token);

private:
	std::string moduleName;
	unsigned int lineOfCode_program;     // LOCpro
	unsigned int lineOfCode_comment;     // LOCcom
	unsigned int lineOfCode_blank;       // LOCbl
	unsigned int lineOfCode_physic;      // LOCphy
	unsigned int mcCabeCyclomaticNumber; // MVG
};

#endif // LOCAL_STAT_H
