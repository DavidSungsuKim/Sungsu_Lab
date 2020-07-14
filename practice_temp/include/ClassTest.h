
#ifndef _CLASS_TEST_H_
#define _CLASS_TEST_H_

class A
{
protected:
	int a = 1;

	virtual void Func1(void) {};
};

class B : protected A
{
public:
	B() {}
	~B() {}
protected:
	B(int i) { /* do something */ }

public:
	int Test() { A::a = 2; return A::a; }

	void Func1(void) {};
};

class C : public B
{
public:
	C() : B(4) {}
	~C() {}

public:
	int Test() { /*B::A::a = 3; */return B::A::a; }

};

class D : public C
{
private:
	D(int a) {};

	int a;
public:
	int Test() { return C::B::A::a; }
};

class CParent1
{
public:
	virtual int GetVal() { return 1; }
};

class CChild1 : public CParent1
{
public:
	int GetVal() { return 2; }
};

class CParent2
{
public:
	virtual int Func1 ()		{ return 1; }
			int Func1 (int x)	{ return x; }
			int Func2 ()		{ return 2; }
};

class CChild2 : public CParent2
{
public:
//	using CParent2::Func1;

	int Func1	()		{ return 10; }
	int Func1	(int x) { return x*10; }
};


class COperator;
const COperator operator+(const COperator& arLeft, const COperator& arRight);

class COperator
{
private:
	friend const COperator operator+(const COperator& arLeft, const COperator& arRight);
	int x;
	int y;

public:
	COperator( )				: x(0), y(0)	{ }
	COperator(int a)			: x(a), y(a)	{ }
 	COperator(int aX, int aY)	: x(aX), y(aY)	{ }
	inline		 COperator& operator  =(const COperator& arRight);
	inline		 COperator& operator +=(const COperator& arRight);
	inline const COperator  operator  +(const COperator& arRight);
};

inline
COperator& COperator::operator=(const COperator& arRight)
{
	if ( this == &arRight )
		return *this;

	x = arRight.x;
	y = arRight.y;
	return *this;
}

inline 
COperator& COperator::operator+=(const COperator& arRight)
{
	x += arRight.x;
	y += arRight.y;
	return *this;
}

inline
const COperator COperator::operator+(const COperator& arRight)
{
	return COperator(x+arRight.x, y+arRight.y);
}

// Non member function
inline 
const COperator operator+(const COperator& arLeft, const COperator& arRight)
{
	return COperator(arLeft.x+arRight.x, arLeft.y+arRight.y);
}

#endif