
#include <stdio.h>
#include <iostream>
#include <vector>
#include "ModernCpp.h"

#define PRINTOUT_FUNC_NAME std::cout << __FUNCTION__ << std::endl; 

void CModernCpp::TypeDeduction()
{
	PRINTOUT_FUNC_NAME;

	int x = 0;

	int* y = &x;
	int z = (int)&y;

	printf("x=0x%x, y=0x%x, z=0x%x\r\n", x, (int)y, z);
}

void func(int x) { /* don't do anything */ }

void CModernCpp::Auto()
{
	PRINTOUT_FUNC_NAME;

#if 0
	auto a = 0;
	auto b = 0.0;

	int sizeA = sizeof(a);
	int sizeB = sizeof(b);
#endif

#if 0
	auto i = 0.1235;
	int size = sizeof(i);
	std::cout <<"auto(i)="<< i <<"  size="<< size << std::endl;
#endif

#if 0
	unsigned char x = 1;
	auto xAuto = x;
	int size = sizeof(xAuto);

	xAuto = 1.3;

	printf("size=%d, xAuto=%.f\r\n", size, xAuto);
#endif

#if 0
	CModernCpp x;

	auto* a = &x;
	a->Auto();
#endif

#if 0
	auto *a = func;
	a(1);
#endif
}

template <typename T>
class CClassForUR
{
public:
	T Test(T&& a)
	{
		return a + 1;
	}
};

void CModernCpp::UniversalReference()
{
	PRINTOUT_FUNC_NAME;

	int i = 0;
	auto& r = i;
	r = 1;        

	auto& r2 = r;
	r2 = 2;        

	auto&& r3 = r;
	r3 = 2;         

#if 0
	auto& r4 = 1;
#endif
	auto&& rr2 = 1;

	CClassForUR<int> my;
	int ret = my.Test(1);
}

void CModernCpp::Initialization()
{
	PRINTOUT_FUNC_NAME;

	auto a = 0;
	auto b = { 0 }; // std::initializer_list<int>
	auto c = { 1, 2 };

//	b = a; // compile rror
	b = c;
}

auto CModernCpp::ReturnAuto( int a )
{	
	PRINTOUT_FUNC_NAME;

	auto ret = a;
	return ret;
}

void CModernCpp::DeclType()
{
	PRINTOUT_FUNC_NAME;

#if 0
	int x = 0;
	decltype(x)	y = 1;

	decltype(0.1) z = 1.2;
#endif

#if 0
	std::vector<int> testList;
	testList.push_back(1);

	int index = 0;
	int value = AuthAndAccess(testList, 0);

	AuthAndAccess(testList, 0) = 3;
	value = testList.at(0);
#endif 

#if 0
	std::vector<int> testList;z
	testList.push_back(1);

	AuthAndAccess2(testList, 0) = 3;

	int value = testList.at(0);
#endif

#if 0
	int x;
	decltype(x) y = 1;
	decltype((x)) z = y;
#endif

	const int x = 0;
	printf("x's name=%s\r\n", typeid(x).name());

	decltype(x) y = 0;

	double d = 1.1;

	decltype(1) z = d;

	int size = sizeof(z);
}

#include <functional>
void PrintWhatYouHave(int a) { printf("a=%d\r\n", a); }
void CModernCpp::StdFunction()
{
	std::function<void(int)> func;

	func = PrintWhatYouHave;
	func(1);
}

void CModernCpp::LamdaExpression()
{
	PRINTOUT_FUNC_NAME;

	int x1 = 1;
	int x2 = 2;

	auto lamdaTest1 = [ ] (int a)		{ printf("lamdaTest1\r\n"); return (a *= 2); };
	
	auto lamdaTest2 = [=](int a) { printf("lamdaTest2\r\n"); return (x1 * 2); };
	
	auto lamdaTest3 = [&] (int a)		{ printf("lamdaTest3\r\n"); return (x1 *= 2); };

	auto lamdaTest4 = [=, &x1](int a)	{ printf("lamdaTest4\r\n"); return (x1 *= (2 * x2)); };

	int ret;
	
	ret = lamdaTest1(2);
	printf("ret=%d, x1=%d, x2=%d\r\n", ret, x1, x2);

	ret = lamdaTest2(2);
	printf("ret=%d, x1=%d, x2=%d\r\n", ret, x1, x2);

	ret = lamdaTest3(2);
	printf("ret=%d, x1=%d, x2=%d\r\n", ret, x1, x2);

	ret = lamdaTest4(2);
	printf("ret=%d, x1=%d, x2=%d\r\n", ret, x1, x2);
}

void CModernCpp::LamdaExpressionForDisassembly()
{
	PRINTOUT_FUNC_NAME;

	int x1 = 1;

	auto lamdaTest1 = [&](int a)
	{
		return (x1 *= 2);
	};

	int ret = lamdaTest1(2);
	printf("ret=%d, x1=%d\r\n", ret, x1);
}

void CModernCpp::ParentheseBraces()
{
	PRINTOUT_FUNC_NAME;

	int x{ 0 }; // braces, allowed only in modern C++
	int y = 0; 
	int z(0); // parentheses

	std::vector<int> test = { 1,2,3 };

	int a = { 1 + 2 + 3 }; // It works!
	int b(1 + 2 + 3);	   // It works either!

	int value;
	CTestClass a1;
	CTestClass a2(10);
	value = a2.getValue();

//	CTestClass a3();	// It's actually for declaring a function!
//	value = a3.getValue();

	CTestClass a4{};
}

void CModernCpp::InitializerList()
{
	PRINTOUT_FUNC_NAME;

	int num;
	CTestClass a1{ 1,2 };
	num = a1.getNumElement();

	CTestClass a2{ 1,2,3,4,5,6 };
	num = a2.getNumElement();

	CTestClass a3{ 1, 2.0 };
	num = a3.getNumElement();

	CTestClass a4{};
	num = a4.getNumElement();
}

void CModernCpp::NullPtr()
{
	PRINTOUT_FUNC_NAME;

	CTestClass  testNull;
	testNull.func(0);

	testNull.func(nullptr);
}

void CModernCpp::ScopedEnums()
{
	PRINTOUT_FUNC_NAME;

#if 0 // not working
	enum color
	{
		black,
		white,
		red
	};

	auto black = 1;
#endif 

#if 0 // 
	enum color
	{
		black,
		white,
		red
	};

	auto anyColor = black;
#endif 

#if 0 // working
	enum class color
	{
		black,
		white,
		red
	};

	auto black = 1;
#endif

#if 0
	enum class color
	{
		black,
		white,
		red
	};

	auto black = black;
#endif

	enum color
	{
		black,
		white,
		red
	};

	int x = color::black;
}

void CModernCpp::Tuple()
{
	PRINTOUT_FUNC_NAME;

	using MyTuple = std::tuple<int, double, long>;

	MyTuple tuple1 = { 1,2,3.4 };

	auto x = std::get<2>(tuple1);
	std::cout << x;
}

void funcNonMember(int a)  {};
void funcNonMember(char a) = delete;
void funcNonMember(bool a) = delete;

void CModernCpp::DeleteForNonMembers()
{
	PRINTOUT_FUNC_NAME;

	funcNonMember(1);
}

class x
{
public:
	void func() { };
	template<typename T>
	void funcDoNotUse(T* ptr) { }
};

template<>
void x::funcDoNotUse<void>(void*) = delete;

void CModernCpp::Delete()
{
	PRINTOUT_FUNC_NAME;

	x x1;
	x1.func();
//	x1.funcDoNotUse();
}

class CBase
{
public:
	virtual int func1(void) const { return 0; }
};

class CDerived : public CBase
{
public:
	int func1(void) const { return 1; } // CBase::func1 À» override
};

void CModernCpp::Override()
{
	PRINTOUT_FUNC_NAME;

	CDerived derived;
	CBase* base = &derived;

	std::cout<<base->func1();
}

void CModernCpp::ConstIterator()
{
	PRINTOUT_FUNC_NAME;

	std::vector<int> myNumbers;
	myNumbers.push_back(1);
	myNumbers.push_back(2);
	myNumbers.push_back(3);

	std::vector<int>::const_iterator it = std::find(myNumbers.cbegin(), myNumbers.cend(), 2);
	myNumbers.insert(it, 0);

	for (it = myNumbers.begin(); it < myNumbers.end(); it++)
	{
		std::cout << *it;
	}
}

class CExceptionSimple
{
public:
	CExceptionSimple(int i) : causeType(i) {}
	int RetrieveCause(void) { return causeType;  }

private:
	int causeType;
};

class ClassNoComplain 
{
public:

#define NO_EXCEPTION
#if defined (NO_EXCEPTION)
	void Func1() noexcept {} 
#else
	void Func1() noexcept { throw CExceptionSimple(1); }
#endif
};

void CModernCpp::NoExcept()
{
	PRINTOUT_FUNC_NAME;

	try
	{
		ClassNoComplain inst;
		inst.Func1();
	}
	catch (CExceptionSimple &a)
	{
		std::cout << "Got an exception";
		std::cout << a.RetrieveCause();
	}

	std::cout << "...end" << std::endl;
}

#include <array>

void CModernCpp::ConstExpr()
{
	PRINTOUT_FUNC_NAME;
#if 1
	constexpr int size = 10;
	std::array<int, size> myArray;
#endif 

#if 0
	const int x = 10;
	constexpr int size = x;
	std::array<int, size> myArray;
#endif 

#if 0
	constexpr int x = 10;
	constexpr int size = x;
	std::array<int, size> myArray;
#endif

#if 0 // not working
	int x = 10;
	const int size = x;
	std::array<int, size> myArray;
#endif

	int parameter = 1;
	int value = GetValue(parameter);
}

#include <mutex>
class CTestStdMutex
{
public:
	struct Time
	{
		int hours;
		int minutes;
		int seconds;
		Time() { hours = 0, minutes = 0, seconds = 0;  }
	};

	CTestStdMutex() : time() {}
	Time GetCurrentTime(void) const
	{
		std::lock_guard<std::mutex> guard(m);		
		if (time.seconds++ > 60)
		{
			time.seconds = 0;
			if (time.minutes++ > 60)
			{
				time.minutes = 0;
				time.hours++;
			}
		}
		return time;
	}

protected:
	mutable std::mutex m;
	mutable struct Time	time;
};

void CModernCpp::StdMutex()
{
	PRINTOUT_FUNC_NAME;
	CTestStdMutex instance;
	instance.GetCurrentTime();
}


class CPerson
{
public:
	CPerson() : height(0), weight(0) {}
	CPerson(int aHeight, int aWeight) : height(aHeight), weight(aWeight) {}
public:
	int height;
	int weight;
};

class CTestStdAtomic
{
public:
	void GetCallCount(void) { count++; }

protected:
	std::atomic<int> count;
	std::atomic<CPerson> myself;
};

#include <atomic>
void CModernCpp::StdAtomic()
{
	PRINTOUT_FUNC_NAME;
	CTestStdAtomic instance;
	instance.GetCallCount();
}

class CTestUniquePtr
{
public:
	CTestUniquePtr() : a(0) { }
	~CTestUniquePtr() { }

	int a;
};

void CModernCpp::UniquePointer()
{
	PRINTOUT_FUNC_NAME;

#if 0 // not recommended
	CTestUniquePtr* instancePtr = new CTestUniquePtr;
	
	std::unique_ptr<CTestUniquePtr> ptr(instancePtr);
	ptr.get()->a++;

	std::unique_ptr<CTestUniquePtr> ptrSame(instancePtr);
	ptrSame.get()->a++;

#endif

#if 0 // should not! 

	CTestUniquePtr instance;
	std::unique_ptr<CTestUniquePtr> ptr(&instance);

	ptr.get()->a = 1;
#endif

#if 0
	std::unique_ptr<CTestUniquePtr> ptr(new CTestUniquePtr);

	{
		ptr.get()->a = 1;
		ptr = std::unique_ptr<CTestUniquePtr>(new CTestUniquePtr);
		ptr.get()->a = 2;
	}
#endif

#if 0
	{
		ptr = std::unique_ptr<CTestUniquePtr>(new CTestUniquePtr);
		ptr.get()->a = 1;
	}
#endif

#if 1
	auto deleter = [](CTestUniquePtr* p)
	{
		std::cout << "deleter called" << std::endl;
		delete p;
	};

	auto deleter2 = [](CTestUniquePtr* p)
	{
		std::cout << "deleter called" << std::endl;
		delete p;
	};
	
	{
		std::unique_ptr<CTestUniquePtr, decltype(deleter)> ptr(new CTestUniquePtr, deleter);
		ptr.get()->a = 1;
	}
#endif


	int x = 0;
}

typedef struct
{
	uint32_t version;
	union
	{
		struct
		{
			uint8_t bit1 : 1;
			uint8_t reserved : 7;
		}bit;
		uint8_t byte;
	}option;

} VERSION;

static VERSION myVersion =
{
	0x11223344,
	0x01,
	0xfe
};

static VERSION myVersion2 =
{
	.version = 0x11223344,
	.option = 0xff
};

void CModernCpp::DesignatedInitializer()
{
	PRINTOUT_FUNC_NAME;
	VERSION versionLocal = myVersion;
}

class ClassForSharedPtr
{
public:
	~ClassForSharedPtr()
	{
		std::cout << __FUNCTION__ << ": data=" << data << std::endl;
	}
	void SetData(int value) { data = value; }

private:
	int data;

};

void CModernCpp::SharedPointer()
{
	PRINTOUT_FUNC_NAME;

	{
		std::shared_ptr<ClassForSharedPtr> ptr1(new ClassForSharedPtr);
		{
			ptr1.get()->SetData(1);
			std::shared_ptr<ClassForSharedPtr> ptr2(ptr1);
			ptr2.get()->SetData(2);
		}
	}

#if 0 // unrecommented usgae
	{
		ClassForSharedPtr* instance = new ClassForSharedPtr;		
		std::shared_ptr<ClassForSharedPtr> ptr1(instance);
		{
			std::shared_ptr<ClassForSharedPtr> ptr2(instance);
		}
	}
#endif

#if 0 // still unrecommented usgae
	{
		ClassForSharedPtr* instance = new ClassForSharedPtr;
		std::shared_ptr<ClassForSharedPtr> ptr1(instance);
		{
			std::shared_ptr<ClassForSharedPtr> ptr2(ptr1);
		}
	}
#endif

	int x = 1;
}

class ClassForPtrs
{
public:
	~ClassForPtrs()
	{
		std::cout << __FUNCTION__ << ": data=" << data << std::endl;
	}
	void SetData(int value) { data = value; }

private:
	int data;
};

void CModernCpp::WeakPointer()
{
	PRINTOUT_FUNC_NAME;

#if 0 // not allowed
	{
		ClassForPtrs* instance = new ClassForPtrs;
		std::weak_ptr<ClassForPtrs> ptrWeak(instance);
	}
#endif

	{
		std::shared_ptr<ClassForPtrs> ptr(new ClassForPtrs);
		std::weak_ptr<ClassForPtrs> ptrWeak(ptr);
		if (ptrWeak.expired())
		{
			printf("expired#1\r\n");
		}

		ptr = 0;// nullptr;

		if (ptrWeak.expired())
		{
			printf("expired#2\r\n");
		}
	}

	{
		std::shared_ptr<ClassForPtrs> ptr1(new ClassForPtrs);		
		std::weak_ptr<ClassForPtrs> ptrWeak(ptr1);
		std::shared_ptr<ClassForPtrs> ptr2 = ptrWeak.lock();
		ptr1 = nullptr;

		if (ptrWeak.expired())
		{
			printf("expired#1\r\n");
		}

		ptr2 = nullptr;
		
		if (ptrWeak.expired())
		{
			printf("expired#2\r\n");
		}
	}

	{
		std::shared_ptr<ClassForPtrs> ptr1(new ClassForPtrs);
		ptr1 = nullptr;
		std::weak_ptr<ClassForPtrs> ptrWeak(ptr1);

		if (ptrWeak.expired())
		{
			printf("expired\r\n");
		}
	}

	{
		std::shared_ptr<ClassForPtrs> ptr1(new ClassForPtrs);
		if (!ptr1.get())
		{
			printf("dangling#1\r\n");
		}

		ptr1 = nullptr;

		if (!ptr1.get())
		{
			printf("dangling#2\r\n");
		}
	}

#if 0 // test
	{
		ClassForPtrs* instance = new ClassForPtrs;
		std::shared_ptr<ClassForPtrs> ptr1(instance);
		std::weak_ptr<ClassForPtrs> ptrWeak(ptr1);

		delete instance;

		if (!ptr1.get())
		{
			printf("dangling#2\r\n");
		}

		if (ptrWeak.expired())
		{
			printf("expired#2\r\n");
		}
	}
#endif
}

void CModernCpp::MakeShared()
{
	PRINTOUT_FUNC_NAME;

	// When using shared_ptr
	{
		std::shared_ptr<ClassForPtrs> ptr1(new ClassForPtrs);
		ptr1.get()->SetData(1);

		std::shared_ptr<ClassForPtrs> ptr2(ptr1);
		ptr1 = nullptr;

		ptr2.get()->SetData(2);
	}

	// When making it shared directly
	{
		auto ptr1 = std::make_shared<ClassForPtrs>();
		ptr1.get()->SetData(1);

		std::shared_ptr<ClassForPtrs> ptr2(ptr1);
		ptr1 = nullptr;

		ptr2.get()->SetData(2);
	}
}

class ClassForMoveForward
{
public:
	ClassForMoveForward(std::string str) : name(str) { printf("%s\r\n", __FUNCTION__); }
	ClassForMoveForward(int value) : data(value) { printf("%s\r\n", __FUNCTION__); }
	ClassForMoveForward(const ClassForMoveForward& rhs)	// copy ctor
	{
		printf("copy ctor\r\n");
	}	

	ClassForMoveForward(ClassForMoveForward&& rhs)		// move ctor
		: name(),
		data(0)
	{
		printf("move ctor\r\n");

		name = std::move(rhs.name);
		rhs.name.empty();
		data = rhs.data;
	}

	std::string name;
	int data;
};

#include <string>
class CTest
{
public:
	CTest(const std::string& name)
		: name(name) { test = 0; }
	CTest(const CTest& lhs) { test = 1; name = lhs.name; }
	CTest& operator =(const CTest& lhs) { test = 2; }
	CTest(CTest&&) { test = 3; }

private:
	std::string name;
	int test;
};

std::list<CTest> testList;
void Function1(const CTest& inst)
{
	testList.push_back(inst);
}

template<typename T>
void Function2(T&& inst)
{
	testList.push_back(std::forward<T>(inst));
}

class Person
{
public:
	template<typename T>
	explicit Person(T&& n)
	{
		T x = n;
	}
};

void CModernCpp::MoveAndForward()
{
	// without std::move()
	{
		ClassForMoveForward a("Jesus Chist The Savior");		// copy ctor is called
		ClassForMoveForward b = a;
	}

	// with std::move()
	{
		ClassForMoveForward a("Jesus Chist The Savior");
		ClassForMoveForward b = std::move(a); // move ctor is called
		b.name = "lll";
		int x = 0;
	}

	{
		ClassForMoveForward a("Jesus Chist The Savior");
		ClassForMoveForward b = std::forward<ClassForMoveForward>(a);
	}

	{
		int value = 1;
		ClassForMoveForward a(value);
		ClassForMoveForward b = std::forward<ClassForMoveForward>(a);
	}

	{
		std::string str1("ABCDEF");
		std::string str3;
		str3 = std::move(str1);
	}

	CTest x("David");
	Function1(x);
	Function2(CTest("Solomon"));

	Person a(0);

	int y = 0;
}

void f(int a, float b)
{
	printf("%d %f", a, b);
}

void f(float a, int b)
{
	printf("%f %d", a, b);
}

template<typename... Ts>
void forwardThis(Ts&&... params)
{
	f(std::forward<Ts>(params)...);
}

void CModernCpp::PerfectForwarding()
{
	forwardThis(int(1), float(0.0));
	forwardThis(float(1), int(0));
}

template<int size>
struct myStruct
{
	static constexpr int m_size = size;
};

template<typename T>
struct myTemplate
{
	T x;
	
	virtual void Func()
	{
		std::cout << "Hey!" << std::endl;
	}
};

struct myChild : public myTemplate<int>
{
	void Func()
	{
		x = 1;
		std::cout << "Hoy!" << std::endl;
	}
};

void CModernCpp::UtilizeTemplate()
{
	myStruct<2> mine;
	int array[mine.m_size];

	constexpr auto size = mine.m_size;
	int array2[size];

	myChild mc;

	myTemplate<int>& ref = mc;
	ref.Func();



}