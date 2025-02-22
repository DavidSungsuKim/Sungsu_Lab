#pragma once

#include <list>
class CTestClass
{
public:
	CTestClass() {};
	CTestClass(int a) { m_a = a; }
	CTestClass(std::initializer_list<long double> il) { m_list = il; }

	~CTestClass() {}

	int getValue()		{ return m_a; }
	int getNumElement()	{ return m_list.size();  }

	void func(int)		{ printf("func#1\r\n"); };
	void func(bool)		{ printf("func#2\r\n"); };
	void func(void*)	{ printf("func#3\r\n"); }

private:
	int m_a;
	std::list<long double> m_list;
};

class CModernCpp
{
public:
	CModernCpp() {}
	~CModernCpp() {}

public:
	void TestRandom();
	void TypeDeduction();
	void Auto();
	void UniversalReference();
	void Initialization();
	auto ReturnAuto( int a );	// not working; need to dig into
	void DeclType();
	void StdFunction();
	void LamdaExpression();
	void LamdaExpressionForDisassembly();
	void ParentheseBraces();
	void InitializerList();
	void NullPtr();
	void ScopedEnums();
	void Tuple();
	void Delete();
	void DeleteForNonMembers();
	void Override();
	void ConstIterator();
	void NoExcept();
	void ConstExpr();
	void StdMutex();
	void StdAtomic();
	void UniquePointer();
	void DesignatedInitializer();
	void SharedPointer();
	void WeakPointer();
	void MakeShared();
	void MoveAndForward();
	void PerfectForwarding();
	void UtilizeTemplate();
	void FunctionObj();
	void ConstIterator2();

	int  GetValue(int parameter) const { return parameter * 2; }
	constexpr int  GetValue(int parameter) { value = 1; return parameter * 2; }

protected:
	int value;
};

template<typename T>
class CModernTemplate
{
public:
	CModernTemplate(T a) { aSaved = a; }
	~CModernTemplate()   { }

	T		Return	(void)				{ return aSaved; };
	void	Func1	(const T& param);
	void	Func2	(T&& param);
	void	Func3	(T param);

private:
	T	aSaved;
};

template<typename T>
void CModernTemplate<T>::Func1(const T& param)
{

}

template<typename T>
void CModernTemplate<T>::Func2(T&& param)
{

}

template<typename T>
void CModernTemplate<T>::Func3(T param)
{
	param = 1;
}

template<typename Container, typename Index>
decltype(auto) AuthAndAccess(Container& c, Index i)
{
	return c[i];
}

template<typename Container, typename Index>
auto& AuthAndAccess2(Container& c, Index i)
{
	return c[i];
}