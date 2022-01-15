
#include <gtest/gtest.h>
#include <math.h>

#include "app.h"
#include "mockHardware.h"

MockHardware *g_mockHardware;

class MyFixture : public ::testing::Test
{
protected:
	void SetUp() override
	{
		i = 1;
		g_mockHardware = &m_mockHardware;
		EXPECT_CALL(m_mockHardware, WriteRegister).WillRepeatedly(testing::Return(0));
	}

	void TearDown() override
	{
		i = 0;
		testing::Mock::VerifyAndClear(&m_mockHardware);
	}

public:
	int i;
	MockHardware m_mockHardware;
};

TEST(HelloTest, TestPow)
{
	double i = pow(2, 2);

	EXPECT_EQ(i, 4);
}

TEST_F(MyFixture, TestUsingFixture)
{
	int j = 1;

	EXPECT_EQ(j, i);
}

TEST_F(MyFixture, TestUsingMock)
{
	int ret = AppFunctionOne();

	EXPECT_EQ(ret, 0);
}