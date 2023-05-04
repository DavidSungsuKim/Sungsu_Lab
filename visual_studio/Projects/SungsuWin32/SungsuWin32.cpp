// SungsuWin32.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <span>

extern "C"
{
	typedef void (*pFunc)(void);
	void Func1	(int x, pFunc fp);
	void Func11	(void);
}

void Func1	(int x, pFunc fp)	{ }
void Func11	(void)				{ }
void Func2	(void)				{ }

#include <stdio.h>
#include <string.h>
#include <memory.h>

void WriteFileNameAndLineFlash(const char* file, int lineNo)
{
   char fileNLine[32] = { 0 };

   char* fileNameOnly = strrchr((char*)file, '\\');
   snprintf(fileNLine, 32, "%s@%d", &fileNameOnly[1], lineNo);
}

class A
{
public:
	A( int id, int param1, int param2 )	
		: m_id( id )
		, m_param1( param1 )
		, m_param2( param2 )
	{

	}
	
	A( int id, A& factoryBase )
		: A( id, factoryBase.m_param1, factoryBase.m_param2 )
	{
	
	}

	void ShowYours()
	{
		printf("id=%d param= %d %d\r\n", m_id, m_param1, m_param2 );
	}

	int m_id;
	int m_param1;
	int m_param2;
};

class B : public A
{

};

A a(0, 100, 200);
A aa(1, a);

enum class MyEnum : unsigned
{
	SIZE = 1
};

template<int size = 2>
struct myTemplate
{
	static constexpr int mySize = size;
};

myTemplate<> myTemps[2];

static myTemplate<>* getMyTemp( int i )
{
	return i == 0 ? &myTemps[0]
		: i == 1 ? &myTemps[1]
		: nullptr;
}

#include <string>
class MemberTemplate
{
public:
	MemberTemplate(unsigned numDev)
		: m_numDev(numDev)
	{ }

	bool checkAges()
	{
		unsigned age;
		return readGeneral([&](auto age) { return readAge(age); }, age);
	}
	
	bool checkNames()
	{
		std::string name;
		return readGeneral( [&](auto name){ return readName( name ); }, name );
	}

	bool readAge(unsigned& age)	{ age = 3; return true; }
	bool readName(std::string &name)	{ name = "sheld"; return true; }

protected:
	template<typename T, typename ReadFunction>
	bool readGeneral(ReadFunction readfunc, T& read)
	{
		for( unsigned i = 0; i < m_numDev; i++ )
		{
			auto ret = readfunc(read);
			if ( ret != true )
			{
				return false;
			}
		}
	}

	const unsigned m_numDev;
};

template<typename T, uint32_t MAX_SIZE>
class StaticArray
{
public:
	T*	getArray()
	{
		return m_array;
	}

	uint32_t getSize()
	{
		return m_size;
	}

protected:
	T m_array[MAX_SIZE];
	constexpr const static uint32_t m_size = MAX_SIZE;
};

#define STR			"WA"
#define NAME(X)	X
#define NAME		STR NAME(1)

int main()
{
	///printf("name=%s\r\n", NAME );

	{
		//StaticArray<int, 4> myArray;
		//auto *array = myArray.getArray();
	}
	
	{
		MemberTemplate	inst(10);

		inst.checkAges();
		inst.checkNames();
	}

	{
		int a = 1;
		int b = 2;

		int * list[] = { &a, &b };

		a = 0;

		class Test
		{
		public:
			Test( int** list )
	      : m_list(list)
			{
				auto *p1 = m_list[0];
				auto* p2 = m_list[1];

				int dummy = 0;
			}

		   int** m_list;
		};

		Test t( list );


	}

	{
		const char str1[] = "abc";
		const char str2[] = "123";

		const char *strList[2];

		strList[0] = str1;
		strList[1] = str2;
	
		int x = 0;

		strList[0] = str2;
	
		x = 1;
	}

	// Span
	{
		int rawArray[2] = { 1, 2 };
		int *rawPtrArray[2] = { &rawArray[0], &rawArray[1] };
		std::span<int*> mySpan{ rawPtrArray, std::size(rawPtrArray) };

		int *test = rawPtrArray[0];
		test++;
		int** test2 = &rawPtrArray[0];
		test2++;
		**test2 = 4;

		*mySpan[0] = 3;
	
		rawArray[0] = 0;
		rawArray[1] = 0;

		auto it = mySpan.begin();
		it++;
		**it = 4;

		int x = 0;
	}

	{
		//int myArray[ MyEnum::SIZE ];

		constexpr int size = 1;
		int myArray2[ size ];

		myTemplate<> myTemp;
		int myArray3[ myTemp.mySize ];			// works fine

		myTemplate<> *pMyTemp = &myTemps[0];	// works fine

		pMyTemp = nullptr;
		pMyTemp = &myTemps[1];
		int myArray4[pMyTemp->mySize];			// works fine

		myTemplate<>* pMyTemp0 = getMyTemp(0); // works fine
		int myAppry5[pMyTemp0->mySize];
	
		//int size2 = pMyTemp0->mySize;
		//int myAppry6[size2];						// doesn't work!
	}


	a.ShowYours();
	aa.ShowYours();

	Func1(1, &Func11);
	Func1(1, &Func2);

	
	WriteFileNameAndLineFlash(__FILE__, __LINE__);

	const char* xxx = "hello";

	int x = 0;

	int* ptr = &x;
	//int* ptr = & x + (int*)xxx;

	return 0;
}