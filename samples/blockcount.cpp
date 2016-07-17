// block = 0
bool function_comparaison(int a, int b)
{
	return (a < b);
}

// block = 1
int function_if(int a, int b)
{
	if (a < b) {
		return a;
	}
	return b;
}

// block = 1
int function_ifelse(int a, int b)
{
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

// block = 1
int function_ifelif(int a, int b, int c)
{
	if (a < b) {
		return -1;
	} else if (a > c){
		return 2;
	} else if (a == b) {
		return 0;
	} else if (a == c) {
		return 1;
	} else {
		return a;
	}
}

// block = 2
int function_ifif(int a, int b, int c)
{
	if (a < b)
		if (b < c)
			return c;
	return a;
}

// block = 1
int function_while(const char *s)
{
	int i = 0;
	while (s[i] != '\0') {
		++i;
	}
	return i;
}

// block = 0
int function_for_noblock(const char *s)
{
	int i;
	for (i = 0; s[i] != '\0'; ++i);
	return i;
}

// block = 2
int function_forfor(const char (&a)[5][5])
{
	int sum = 0;
	for (int j = 0; j != 5; ++j) {
		for (int i = 0; i != 5; ++i) {
			sum += a[i][j];
		}
	}
	return sum;
}

// block = 0
int function_forfor_no_block(const char (&a)[5][5])
{
	int sum = 0;
	for (int j = 0; j != 5; ++j)
		for (int i = 0; i != 5; ++i)
			sum += a[i][j];
	return sum;
}

// block = 1
const char* function_switch(int i)
{
	switch (i) {
		case 0: return "monday";
		case 1: return "tuesday";
		case 2: return "wesnesday";
		case 3: return "thursday";
		case 4: return "friday";
		case 5:
		case 6: return "week end";
		default: return 0;
	}
}

// block = 2
void function_block()
{
	int whichvalue = 0;
	{
		int autoblock = 2;
		{
			int autoblock = 4;
			whichvalue += autoblock;
		}
		whichvalue += autoblock;
	}
	whichvalue = 42;
}
