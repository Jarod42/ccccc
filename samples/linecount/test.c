#define SIX 1 + 5
#define NINE 8 + 1

int main(int argc, char** argv);

/* multi
  line
  comment

  Previous line is not a blank line but a comment
*/
int main(int argc, char** argv)
{
	int six = SIX;
	int nine = NINE; // some comment
	int prod = SIX * NINE;
	/* 1rst comment */  // Some comment

	if (prod != six * nine)
		return 1;
	return 0;
}
