
#ifndef TEST_H
#define TEST_H

int mult(int lhs, int rhs);

inline int mult(int lhs, int rhs)
{
	return lhs * rhs;
}

#endif
