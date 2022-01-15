
#include "gmock/gmock.h"

class HardwareIF
{
public:
	virtual int WriteRegister(int value) = 0;
};

class MockHardware : public HardwareIF
{
public:
	MOCK_METHOD(int, WriteRegister, (int));
};

extern MockHardware *g_mockHardware;

