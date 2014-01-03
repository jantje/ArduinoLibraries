/*
 * wiebel.h
 *
 *  Created on: Aug 27, 2013
 *      Author: jan
 */

#ifndef WIEBEL_H_
#define WIEBEL_H_
#include "Arduino.h"
#include "Servo.h"

class wiebel {
public:
	enum actieStatus{niks_te_doen,naar_groter,naar_kleiner,midden_positie_naar_kleinst,midden_positie_naar_grootst,helemaal_grootsts,helemaal_kleinst};
private:
	uint32_t startTijd;
	uint8_t mijnStartPositie;
	uint8_t mijnWielbelStartPlaats;
	uint8_t mijnHuidigePositie;
	actieStatus	mijnActieStatus;
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
	Servo mijnServo;
	uint8_t mijnServoPin;
	const char * myname;
public:
	wiebel(	const char * name, uint8_t ServoPin);
	void setup();
	void loop();
	void startWiebel(uint8_t grootsteHoek,uint8_t kleinsteHoek,uint16_t tijdNaarGrootsteHoek,uint16_t tijdNaarKleinsteHoek,uint16_t wachtTijdGrootsteHoek,uint16_t wachtTijdKleinsteHoek,uint16_t startPlaats_pause_naar_grootst,uint16_t startPlaats_pause_naar_kleinst,uint16_t herhalingen);
	uint8_t getPositie(){return mijnHuidigePositie;};
	void setPositie(uint8_t nieuwe);
	actieStatus getActieStatus(){return mijnActieStatus;};
	const char * getStatusName();
};

#endif /* WIEBEL_H_ */
