// block = 1
bool function_comparaison(int a, int b)
{
	return (a < b);
}

// block = 2
int function_if(int a, int b)
{
	if (a < b) {
		return a;
	}
	return b;
}

// block = 2
int function_ifelse(int a, int b)
{
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

// block = 2
int function_while(const char *s)
{
	int i = 0;
	while (s[i] != '\0') {
		++i;
	}
	return i;
}

// block = 1
int function_for_noblock(const char *s)
{
	int i;
	for (i = 0; s[i] != '\0'; ++i);
	return i;
}

// block = 1
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

// block = 2
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
		default: return NULL;
	}
}
