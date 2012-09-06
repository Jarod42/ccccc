
#ifndef LOCAL_STAT_H
#define LOCAL_STAT_H

class LocalStatTool;

class LocalStat
{
	friend class LocalStatTool;
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

#endif // LOCAL_STAT_H
