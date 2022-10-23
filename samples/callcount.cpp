#if 1 // ADL
struct S
{};

void ADL(void*)
{}

template <typename T>
void f(T& t)
{
	S s;
	ADL(&s); // ADL(void*)
	ADL(&t); // ADL(S*)
}

void ADL(S*)
{}

void callADL(S& s)
{
	f(s); // f<S>
}
#endif
#if 1 // operator
struct Operator
{
	operator int() const { return 42; }
	const Operator& operator*() const { return *this; }
};

void UseOperator(Operator& op)
{
	int n = op; // operator int()
	*op;
}
#endif

#if 1 // function pointer
void call(void (*f)())
{
	f();
}

void someFunc()
{}

void callSomeFunc()
{
	call(&someFunc);
}

#endif
#if 1 // constructor/destructor
struct Implicit
{};
void ImplicitCount()
{
	Implicit c1;
	Implicit c2(c1);
}
struct Default
{
	Default() = default;
	~Default() = default;
	Default(const Default&) = default;
};
void DefaultCount()
{
	Default c1;
	Default c2(c1);
}
struct UserProvided
{
	UserProvided() {}
	~UserProvided() {}
	UserProvided(const UserProvided&) {}
};
void UserProvidedCount()
{
	UserProvided c1;
	UserProvided c2(c1);
}
#endif
#if 1 // new/delete
class New
{};
void newDelete()
{
	delete new New;
}
#endif

#if 1 // function as parameter
void function_as_parameter()
{
	int f();
	void g(int, int, int, int);
	g(f(), f(), f(), f());
}
#endif

#if 1 // main
int main()
{}
#endif
