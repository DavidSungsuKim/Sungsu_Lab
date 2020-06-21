
#ifndef _CLASS_TEST_H_
#define _CLASS_TEST_H_

class A
{
public:
	A() {}
	~A() {}

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

class D
{
private:
	D(int a) {};

	int a;
};

#endif