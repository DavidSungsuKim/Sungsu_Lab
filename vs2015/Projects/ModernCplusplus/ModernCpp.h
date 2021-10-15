#pragma once

class CModernCpp
{
public:
	CModernCpp() {}
	~CModernCpp() {}

public:
	void TypeDeduction();
	void Auto();
	void UniversalReference();
	void Initialization();
	auto ReturnAuto( int a );	// not working; need to dig into
	void DeclType();
	void StdFunction();
	void LamdaExpression();
	void LamdaExpressionForDisassembly();
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