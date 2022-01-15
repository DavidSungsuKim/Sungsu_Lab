
#include "gmock/gmock.h"

class HardwareIF
{
public:
	virtual int WriteRegister(int value) = 0;
};

class MockHardware : public HardwareIF
{
public:
	MOCK_METHOD1(WriteRegister, int(int));
};

extern MockHardware *g_mockHardware;

