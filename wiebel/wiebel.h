/*
 * wiebel.h
 *
 *  Created on: Aug 27, 2013
 *      Author: jan
 */

#ifndef WIEBEL_H_
#define WIEBEL_H_
#include "Arduino.h"
#include "JanServo.h"

enum actieStatus
{
	niks_te_doen, naar_groter, naar_kleiner, midden_positie_naar_kleinst, midden_positie_naar_grootst, helemaal_grootsts, helemaal_kleinst
};
class wiebel
{
	private:
		//uint32_t startTijd;
		uint8_t mijnStartPositie;
		uint8_t mijnWielbelStartPlaats;
		uint8_t mijnHuidigePositie;
		actieStatus mijnActieStatus;
		uint8_t mijnGrootsteHoek;
		uint8_t mijnKleinsteHoek;
		uint16_t mijnTijdNaarGroter;
		uint16_t mijnTijdNaarKleiner;
		uint16_t mijnWachtTijdGrootsteHoek;
		uint16_t mijnWachtTijdKleinsteHoek;
		uint16_t mijnWiebelStartPlaats_pause_naar_grootst;
		uint16_t mijnWiebelStartPlaats_pause_naar_kleinst;
		uint16_t mijnHerhalingen;
		uint32_t mijnStartTijd;
		iServo * mijnServo1;
		uint32_t lastlogTime;
	public:
		wiebel();
		void setup();
		void loop();
		void startWiebel(iServo* mijnServo, uint8_t kleinsteHoek,uint8_t grootsteHoek, uint16_t tijdNaarGrootsteHoek, uint16_t tijdNaarKleinsteHoek, uint16_t wachtTijdGrootsteHoek, uint16_t wachtTijdKleinsteHoek, uint16_t startPlaats_pause_naar_grootst, uint16_t startPlaats_pause_naar_kleinst,
				uint16_t herhalingen);
		uint8_t getPositie()
		{
			return mijnHuidigePositie;
		}
		void setPositie(uint8_t nieuwe);
		actieStatus getActieStatus()
		{
			return mijnActieStatus;
		}
		const char * getStatusName();
};



class draaier
{
	private:
		actieStatus mijnActieStatus;
		iServo* myWiebelServo;
		iServo* myBuigServo;
		uint8_t mijnStartHellingHoek;
		uint8_t mijnEindHellingHoek;
		uint8_t mijnStartDraaiHoek;
		uint8_t mijnEindDraaiHoek;
		uint32_t mijnLoopTijd;
		int16_t mijnAantalRondjes;
		uint32_t myStarttime;
		int32_t myTotalDegrees;
		int32_t myTotalHeightDegrees;
		int32_t previousHightCorner ;
		int32_t previousDirectionCorner ;

	public:
		draaier(){mijnActieStatus=niks_te_doen;}
		actieStatus getActieStatus()
		{
			return mijnActieStatus;
		}
		void startRondjes(iServo* Servo1, iServo* Servo2, uint8_t startHellingHoek, uint8_t eindHellingHoek, uint8_t startDraaiHoek, uint8_t eindDraaiHoek, uint32_t tijd, int16_t aantalRondjes);
		void setup(){};
		void loop();
};

#endif /* WIEBEL_H_ */
