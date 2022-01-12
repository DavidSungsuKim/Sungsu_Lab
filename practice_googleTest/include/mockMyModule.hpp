
#include <gmock/gmock.h>
#include "myModule.hpp"

class MyModule
{
public:
	virtual int returnOne(void) = 0;
};

class MockMyModule : public MyModule
{
public:
	MOCK_METHOD0(returnOne, int(void));
};

extern MockMyModule mockInstance;

