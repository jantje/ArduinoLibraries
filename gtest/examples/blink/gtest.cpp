#ifdef UNITTEST
#include "gtest.h"
#include "blink.h"

TEST(blink, start) {
	EXPECT_EQ(HIGH, calcLedState(0, HIGH));
	EXPECT_EQ(LOW, calcLedState(0, LOW));
	EXPECT_EQ(HIGH, calcLedState(1, HIGH));
	EXPECT_EQ(LOW, calcLedState(1, LOW));
//	EXPECT_EQ(2, calcLedState(INTERVAL-2, HIGH));
//	EXPECT_EQ(2, calcLedState(INTERVAL-1, HIGH));
//	EXPECT_EQ(2, calcLedState(INTERVAL, HIGH));
//	EXPECT_EQ(2, calcLedState(INTERVAL+1, HIGH));
//	EXPECT_EQ(2, calcLedState(INTERVAL+2, HIGH));

}

TEST(blink, on) {
	uint32_t mill=0;
	for (mill = 0; mill < INTERVAL; mill++) {
		EXPECT_EQ(HIGH, calcLedState(mill, HIGH)) << mill;
	}
	EXPECT_EQ(LOW, calcLedState(INTERVAL, HIGH)) << INTERVAL;

}

TEST(blink, off) {
	uint32_t mill=0;
	for (mill = 0; mill < INTERVAL; mill++) {
		EXPECT_EQ(LOW, calcLedState(INTERVAL+mill, LOW)) << INTERVAL+mill;
	}
	EXPECT_EQ(HIGH, calcLedState(2*INTERVAL, LOW)) << INTERVAL;
}

#endif
