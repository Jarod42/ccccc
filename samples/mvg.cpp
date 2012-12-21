#include <stddef.h> // NULL
#include <utility> // std::move

// mvg = 1
bool function_comparaison(int a, int b)
{
	return (a < b);
}

// mvg = 2
int function_if(int a, int b)
{
	if (a < b) {
		return a;
	}
	return b;
}

// mvg = 2
int function_ifelse(int a, int b)
{
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

// mvg = 2
int function_ternaire(int a, int b)
{
	return (a < b) ? a : b;
}

// mvg = 2
int function_while(const char *s)
{
	int i = 0;
	while (s[i] != '\0') {
		++i;
	}
	return i;
}

// mvg = 2
int function_for(const char *s)
{
	int i;
	for (i = 0; s[i] != '\0'; ++i);
	return i;
}

// mvg = 8
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

// mvg = 1
bool function_and(int low, int mid, int high)
{
	return (low <= mid) & (mid <= high);
}
// mvg = 2
bool function_andand(int low, int mid, int high)
{
	return low <= mid && mid <= high;
}

// mvg = 1
bool function_or(int low, int mid, int high)
{
	return (mid < low) | (high < mid);
}
// mvg = 2
bool function_oror(int low, int mid, int high)
{
	return mid < low || high < mid;
}

// mvg = 1
template<typename T>
void function_rval(T& a, T& b)
{
	T&& rval = std::move(a);
	T c(rval);
	a = std::move(b);
	b = c;
}

// mvg = 1
void function_rval(int&& a)
{
}
