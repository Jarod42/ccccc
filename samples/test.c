#define SIX 1 + 5
#define NINE 8 + 1

#include "stdlib.h"

int main(int argc, char** argv);

/* multi
  line
  comment
*/
int main(int argc, char** argv)
{
	int six = SIX;
	int nine = NINE; // some comment
	int prod = SIX * NINE;
	/* 1rst comment */  // Some comment

	if (prod != six * nine)
		return 0;
	return 1;
}
