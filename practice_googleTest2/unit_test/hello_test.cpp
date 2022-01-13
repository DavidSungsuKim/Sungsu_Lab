
#include <gtest/gtest.h>
#include <math.h>

#include "app.hpp"

class MyFixture : public ::testing::Test
{
protected:
	void SetUp() override
	{
		i = 1;
	}

	void TearDown() override
	{
		i = 0;
	}

public:
	int i;
};

TEST_F(MyFixture, TestUsingFixture)
{
	int j = i;

	int k = AppFunctionOne();

	EXPECT_EQ(j, k);
}

TEST(HelloTest, TestPow)
{
	double i = pow(2, 2);

	EXPECT_EQ(i, 4);
}