#include <stddef.h>
#include <algorithm>


class Foo
{
public:
	Foo() : i(42), mem(new char[42]) {}

	Foo(Foo &&rhs) : i(42), mem(NULL)
	{
		std::swap(mem, rhs.mem);
	}

	int bar()
	{
		i = isNul() ? 0 : i - 1;
		return i;
	}

	bool isNul() const;

private:
	int i;
	char * mem;
};

bool Foo::isNul() const
{
	return i == 0;
}
