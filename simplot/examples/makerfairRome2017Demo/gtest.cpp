#ifdef UNITTEST
#include "gtest.h"
#include "makerfairRome2017Demo.h"


TEST(getBlinkLedValue, randomValues)
{
	EXPECT_EQ(getBlinkLedValue(0,200,300),1);
	EXPECT_EQ(getBlinkLedValue(150,200,300),1);
	EXPECT_EQ(getBlinkLedValue(199,200,300),1);
	EXPECT_EQ(getBlinkLedValue(200,200,300),0);
	EXPECT_EQ(getBlinkLedValue(201,200,300),0);
	EXPECT_EQ(getBlinkLedValue(300,200,300),0);
	EXPECT_EQ(getBlinkLedValue(400,200,300),0);
	EXPECT_EQ(getBlinkLedValue(499,200,300),0);
	EXPECT_EQ(getBlinkLedValue(500,200,300),1);
	EXPECT_EQ(getBlinkLedValue(699,200,300),1);
	EXPECT_EQ(getBlinkLedValue(700,200,300),0);

}

TEST(getFadeLedValue, randomValues)
{
	int error=5;
	EXPECT_NEAR(getFadeLedValue(0,200,300),0,error);
	EXPECT_NEAR(getFadeLedValue(100,200,300),125,error);
	EXPECT_NEAR(getFadeLedValue(150,200,300),190,error);
	EXPECT_NEAR(getFadeLedValue(199,200,300),255,error);
	EXPECT_NEAR(getFadeLedValue(200,200,300),255,error);
	EXPECT_NEAR(getFadeLedValue(201,200,300),255,error);
	EXPECT_NEAR(getFadeLedValue(300,200,300),170,error);
	EXPECT_NEAR(getFadeLedValue(400,200,300),85,error);
	EXPECT_NEAR(getFadeLedValue(499,200,300),0,error);
	EXPECT_NEAR(getFadeLedValue(500,200,300),0,error);
	EXPECT_NEAR(getFadeLedValue(699,200,300),255,error);
	EXPECT_NEAR(getFadeLedValue(700,200,300),255,error);

}





#endif
