
#include <gmock/gmock.h>

#include "include/mockMyModule.hpp"
#include "include/myModule.hpp"

MockMyModule mockInstance;

int MyModule::returnOne()
{
	return mockInstance.returnOne();
}