#define SIX 1 + 5
#define NINE 8 + 1

#include "test.h"

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
	int expected = mult(six, nine);
	/* 1rst comment */  // Some comment

	if (prod != expected)
		return 1;
	return 0;
}
