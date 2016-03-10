/*
 * wiebel.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: jan
 */

#include "wiebel.h"

wiebel::wiebel()
{
	mijnStartPositie = 0;
	mijnHuidigePositie = 0;
	mijnActieStatus = niks_te_doen;
	mijnGrootsteHoek = 0;
	mijnKleinsteHoek = 0;
	mijnTijdNaarGroter = 0;
	mijnTijdNaarKleiner = 0;
	mijnWachtTijdGrootsteHoek = 0;
	mijnWachtTijdKleinsteHoek = 0;
	mijnWielbelStartPlaats = 0;
	mijnWiebelStartPlaats_pause_naar_grootst = 0;
	mijnWiebelStartPlaats_pause_naar_kleinst = 0;
	mijnHerhalingen = 0;
	mijnStartTijd = millis();
}

void wiebel::setup()
{
	lastlogTime=0;
}

void wiebel::loop()
{

	switch (mijnActieStatus)
	{

		case niks_te_doen:
			break;
		case naar_groter:
		{
			//waar moest ik al zijn
			uint8_t nieuwePositie = min( mijnGrootsteHoek,mijnStartPositie
					+ (((mijnGrootsteHoek - mijnKleinsteHoek)
							* (millis() - mijnStartTijd)) / mijnTijdNaarGroter));


			nieuwePositie=max(nieuwePositie,mijnHuidigePositie);
			if (mijnHuidigePositie < mijnWielbelStartPlaats && nieuwePositie >= mijnWielbelStartPlaats)
			{
				nieuwePositie = mijnWielbelStartPlaats;
				mijnActieStatus = midden_positie_naar_grootst;
				mijnStartTijd = millis();
			}
			mijnHuidigePositie = nieuwePositie;
			// zet je daar
			mijnServo1->setPosition(mijnHuidigePositie);
			// moet ik keren?
			if (mijnHuidigePositie >= mijnGrootsteHoek)
			{
				mijnActieStatus = helemaal_grootsts;
			}
			break;
		}
		case midden_positie_naar_grootst:
		{
			if (millis() - mijnStartTijd > mijnWiebelStartPlaats_pause_naar_grootst)
			{
				mijnStartTijd += mijnWiebelStartPlaats_pause_naar_grootst;
				mijnActieStatus = naar_groter;
			}
			break;
		}
		case helemaal_grootsts:
		{

			if (millis() - mijnStartTijd >= (mijnTijdNaarGroter + mijnWachtTijdGrootsteHoek))
			{
				mijnActieStatus = naar_kleiner;
				mijnStartTijd = millis();
			}
			break;
		}
		case naar_kleiner:
		{

			//waar moest ik al zijn
			uint8_t nieuwePositie = max(mijnKleinsteHoek, mijnGrootsteHoek
					- (((mijnGrootsteHoek - mijnKleinsteHoek)
							* (millis() - mijnStartTijd)) / mijnTijdNaarKleiner));
			nieuwePositie=min(nieuwePositie,mijnHuidigePositie);
			if (mijnHuidigePositie > mijnWielbelStartPlaats && nieuwePositie <= mijnWielbelStartPlaats)
			{
				nieuwePositie = mijnWielbelStartPlaats;
				mijnActieStatus = midden_positie_naar_kleinst;
				mijnStartTijd = millis();
			}
			mijnHuidigePositie = nieuwePositie;

			// moet ik keren?
			if (mijnHuidigePositie <= mijnKleinsteHoek)
			{
				mijnHuidigePositie = mijnKleinsteHoek;
				mijnActieStatus = helemaal_kleinst;
			}
			// zet je daar
			mijnServo1->setPosition(mijnHuidigePositie);
			break;
		}
		case midden_positie_naar_kleinst:
		{
			if (millis() - mijnStartTijd >= mijnWiebelStartPlaats_pause_naar_kleinst)
			{
				mijnStartTijd += mijnWiebelStartPlaats_pause_naar_kleinst;
				mijnActieStatus = naar_kleiner;
			}
			break;
		}
		case helemaal_kleinst:
		{

			if (millis() - mijnStartTijd >=  mijnWachtTijdKleinsteHoek)
			{
				mijnActieStatus = naar_groter;
				mijnStartTijd = millis();
				mijnStartPositie = mijnKleinsteHoek;
				if (--mijnHerhalingen == 0)
				{
					mijnActieStatus = niks_te_doen;
					//mijnGrootsteHoek = mijnWielbelStartPlaats;

				}
			}
			break;
		}

	}

//	if (millis() - lastlogTime > 1000)
	if (mijnActieStatus != niks_te_doen)
	{
		lastlogTime=millis();
		Serial.print("pin: ");
		Serial.print((long)mijnServo1);



		Serial.print(" mijnKleinsteHoek: ");
		Serial.print(mijnKleinsteHoek);
		Serial.print("mijnGrootsteHoek: ");
		Serial.print(mijnGrootsteHoek);
		Serial.print(" mijnTijdNaarGroter: ");
		Serial.print(mijnTijdNaarGroter);
		Serial.print(" mijnTijdNaarKleiner: ");
		Serial.print(mijnTijdNaarKleiner);

		Serial.print(" mijnWachtTijdGrootsteHoek: ");
		Serial.print(mijnWachtTijdGrootsteHoek);

		Serial.print(" mijnWachtTijdKleinsteHoek: ");
		Serial.print(mijnWachtTijdKleinsteHoek);

		Serial.print(" mijnWiebelStartPlaats_pause_naar_kleinst: ");
		Serial.print(mijnWiebelStartPlaats_pause_naar_kleinst);


		Serial.print(" her: ");
		Serial.print(mijnHerhalingen);
		Serial.print(" pos: ");
		Serial.print(mijnHuidigePositie);
		Serial.print(" status: ");
		Serial.println(getStatusName());
		Serial.println(millis());
	}
}

void wiebel::startWiebel(iServo* Servo,  uint8_t kleinsteHoek, uint8_t grootsteHoek, uint16_t tijdNaarGrootsteHoek, uint16_t tijdNaarKleinsteHoek, uint16_t wachtTijdGrootsteHoek, uint16_t wachtTijdKleinsteHoek, uint16_t startPlaats_pause_naar_grootst, uint16_t startPlaats_pause_naar_kleinst,
		uint16_t herhalingen)
{

	mijnServo1 = Servo;
	mijnStartPositie = mijnHuidigePositie;
	mijnWielbelStartPlaats = mijnHuidigePositie;
	mijnActieStatus = naar_groter;
	mijnGrootsteHoek = grootsteHoek;
	mijnKleinsteHoek = kleinsteHoek;
	mijnTijdNaarGroter = tijdNaarGrootsteHoek;
	mijnTijdNaarKleiner = tijdNaarKleinsteHoek;
	mijnWachtTijdGrootsteHoek = wachtTijdGrootsteHoek;
	mijnWachtTijdKleinsteHoek = wachtTijdKleinsteHoek;
	mijnWiebelStartPlaats_pause_naar_grootst = startPlaats_pause_naar_grootst;
	mijnWiebelStartPlaats_pause_naar_kleinst = startPlaats_pause_naar_kleinst;
	mijnHerhalingen = herhalingen;
	lastlogTime=mijnStartTijd = millis();
}

void wiebel::setPositie(uint8_t nieuwe)
{
	mijnHuidigePositie = nieuwe;

	// zet je daar
	mijnServo1->setPosition(mijnHuidigePositie);
}

const char * wiebel::getStatusName()
{
	switch (mijnActieStatus)
	{
		case niks_te_doen:
			return "niks_te_doen";
		case naar_groter:
			return "naar_groter";
		case naar_kleiner:
			return "naar_kleiner";
		case midden_positie_naar_kleinst:
			return "midden_positie_naar_kleinst";
		case midden_positie_naar_grootst:
			return "midden_positie_naar_grootst";
		case helemaal_grootsts:
			return "helemaal_grootsts";
		case helemaal_kleinst:
			return "helemaal_kleinst";
	}
	return "unnamed state";
}

void draaier::startRondjes(iServo* wiebelServo, iServo* buigServo, uint8_t startHellingHoek, uint8_t eindHellingHoek, uint8_t startDraaiHoek, uint8_t eindDraaiHoek, uint32_t tijd, int16_t aantalRondjes)
{
	myWiebelServo = wiebelServo;
	myBuigServo = buigServo;
	mijnStartHellingHoek = startHellingHoek;
	mijnEindHellingHoek = eindHellingHoek;
	mijnStartDraaiHoek = startDraaiHoek;
	mijnEindDraaiHoek = eindDraaiHoek;
	mijnLoopTijd = tijd;
	mijnAantalRondjes = aantalRondjes;
	mijnActieStatus=naar_groter;
	myTotalDegrees=360*aantalRondjes;
	myTotalDegrees+=eindDraaiHoek;
	myTotalDegrees-=startDraaiHoek;
	myTotalHeightDegrees=eindHellingHoek;
	myTotalHeightDegrees-=(int32_t)startHellingHoek;
	myStarttime=millis();
	previousHightCorner =0;
	previousDirectionCorner =0;


}


int plotBuffer[20];
void plot(int data1, int data2, int data3, int data4, int data5, int data6)
{
  int pktSize;

  plotBuffer[0] = 0xCDAB;             //SimPlot packet header. Indicates start of data packet
  plotBuffer[1] = 6*sizeof(int);      //Size of data in bytes. Does not include the header and size fields
  plotBuffer[2] = data1;
  plotBuffer[3] = data2;
  plotBuffer[4] = data3;
  plotBuffer[5] = data4;
  plotBuffer[6] = data5;
  plotBuffer[7] = data6;

  pktSize = 2 + 2 + (6*sizeof(int)); //Header bytes + size field bytes + data

  //IMPORTANT: Change to serial port that is connected to PC
  Serial.write((uint8_t * )plotBuffer, pktSize);
}


void draaier::loop()
{
	switch (mijnActieStatus)
	{
		case niks_te_doen:
		case naar_kleiner:
		case midden_positie_naar_kleinst:
		case midden_positie_naar_grootst:
		case helemaal_grootsts:
		case helemaal_kleinst:
			break;
		case naar_groter:
		{

			uint32_t now=millis();
			uint32_t runTime = now-myStarttime;
			int32_t newHightCorner =   mijnStartHellingHoek;
			newHightCorner += ((int32_t)myTotalHeightDegrees * (int32_t)runTime)/(int32_t)mijnLoopTijd;
			int32_t newDirectionCorner = mijnStartDraaiHoek;
			newDirectionCorner += ((int32_t) myTotalDegrees*(int32_t)runTime/(int32_t)mijnLoopTijd);
			if ((newHightCorner != previousHightCorner) || (previousDirectionCorner != newDirectionCorner))
			{
				int16_t wiebelservoPositie= cos((float)newDirectionCorner*DEG_TO_RAD)*90 *sin((float)newHightCorner*DEG_TO_RAD) +90;
				int16_t buigservoPositie=  sin((float)newDirectionCorner*DEG_TO_RAD)*90 *sin((float)newHightCorner*DEG_TO_RAD) +90;
				myWiebelServo->setPosition(wiebelservoPositie);
				myBuigServo->setPosition(buigservoPositie);
				plot(newDirectionCorner,0,0,myTotalDegrees,wiebelservoPositie,buigservoPositie);

				previousHightCorner = newHightCorner;
				previousDirectionCorner = newDirectionCorner;
			}
			if(millis() -myStarttime>= mijnLoopTijd)
			{
				Serial.println("Draaier niks te doen");
				mijnActieStatus=niks_te_doen;
				previousHightCorner = 0;
				previousDirectionCorner = 0;
			}
			break;
		}
	}

}
