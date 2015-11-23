#ifdef UNITTEST
#include "gtest.h"
#include "your header"


TEST(Security, encryption)
{
	PermissionHolder permission;
	DateTime testTime(2015,6,13,12,12,12);
//  DateTime (uint16_t year, uint8_t month, uint8_t day,
//              uint8_t hour =0, uint8_t min =0, uint8_t sec =0);
	int RobotID=1;
	permission.requestPermission(Cassis_terras,testTime,RobotID);
	memcpy(permission.answerCipher,permission.sendCipher,sizeof(permission.answerCipher));
	permission.loop();
	String answer=permission.answerPlain;
	String send=permission.sendPlain;
	EXPECT_EQ(answer,send);
}

TEST(gps, convertDegreesToDeci)
{
//	GPSLocation in0 =
//			{ 51031580, 3421810 };
//	GPSLocation out0 =
//			{ 51052633, 3703016 }; 

//
//	in0.convertDegreesToDeci();
//	EXPECT_EQ(out0.myLatitude, in0.myLatitude);
//	EXPECT_EQ(out0.myLongitude, in0.myLongitude);
}

TEST(gps, distance)
{
//	GPSLocation dist1_1 ={ 51057580, 3699610 };
//	GPSLocation dist1_2 ={ 51054420, 3704980 };
//	long dist1 = 51406; //in cm 51410; //in cm

//	EXPECT_EQ(0, dist1_1.distance(dist1_1));
//	EXPECT_EQ(0, dist1_2.distance(dist1_2));
//	EXPECT_EQ(dist1, dist1_2.distance(dist1_1));
//	EXPECT_EQ(dist1, dist1_1.distance(dist1_2));

}



#endif
