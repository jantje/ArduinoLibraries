/*
 * wiebel.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: jan
 */

#include "wiebel.h"

wiebel::wiebel(const char* name,uint8_t ServoPin) {
	myname =name;
	mijnServoPin = ServoPin;
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

void wiebel::setup() {
	mijnServo.attach(mijnServoPin);
}

void wiebel::loop() {

	switch (mijnActieStatus) {

	case niks_te_doen:
		break;
	case naar_groter: {
		//waar moest ik al zijn
		uint8_t nieuwePositie =
				min( mijnGrootsteHoek,mijnStartPositie
						+ ((mijnGrootsteHoek - mijnStartPositie)
								* (millis() - mijnStartTijd) / mijnTijdNaarGroter));
		if (mijnHuidigePositie < mijnWielbelStartPlaats
				&& nieuwePositie >= mijnWielbelStartPlaats) {
			nieuwePositie = mijnWielbelStartPlaats;
			mijnActieStatus = midden_positie_naar_grootst;
			startTijd = millis();
		}
		mijnHuidigePositie = nieuwePositie;
		// zet je daar
		mijnServo.write(mijnHuidigePositie);
		// moet ik keren?
		if (mijnHuidigePositie >= mijnGrootsteHoek) {
			mijnActieStatus = helemaal_grootsts;
		}
		break;
	}
	case midden_positie_naar_grootst: {
		if (millis() - startTijd > mijnWiebelStartPlaats_pause_naar_grootst) {
			mijnStartTijd += mijnWiebelStartPlaats_pause_naar_grootst;
			mijnActieStatus = naar_groter;
		}
		break;
	}
	case helemaal_grootsts: {

		if (millis() - mijnStartTijd
				> (mijnTijdNaarGroter + mijnWachtTijdGrootsteHoek)) {
			mijnActieStatus = naar_kleiner;
			mijnStartTijd = millis();
		}
		break;
	}
	case naar_kleiner: {

		//waar moest ik al zijn
		uint8_t nieuwePositie =
				max(mijnKleinsteHoek, mijnGrootsteHoek
						- ((mijnGrootsteHoek - mijnKleinsteHoek)
								* (millis() - mijnStartTijd) / mijnTijdNaarKleiner));
		if (mijnHuidigePositie > mijnWielbelStartPlaats
				&& nieuwePositie <= mijnWielbelStartPlaats) {
			nieuwePositie = mijnWielbelStartPlaats;
			mijnActieStatus = midden_positie_naar_kleinst;
			startTijd = millis();
		}
		mijnHuidigePositie = nieuwePositie;


		// moet ik keren?
		if (mijnHuidigePositie <= mijnKleinsteHoek) {
			mijnHuidigePositie = mijnKleinsteHoek;
			mijnActieStatus = helemaal_kleinst;
		}
		// zet je daar
		mijnServo.write(mijnHuidigePositie);
		break;
	}
	case midden_positie_naar_kleinst: {
		if (millis() - startTijd > mijnWiebelStartPlaats_pause_naar_kleinst) {
			mijnStartTijd += mijnWiebelStartPlaats_pause_naar_kleinst;
			mijnActieStatus = naar_kleiner;
		}
		break;
	}
	case helemaal_kleinst: {

		if (millis() - mijnStartTijd
				> (mijnTijdNaarKleiner + mijnWachtTijdKleinsteHoek)) {
			mijnActieStatus = naar_groter;
			mijnStartTijd = millis();
			mijnStartPositie = mijnKleinsteHoek;
			if (--mijnHerhalingen == 0) {
				mijnActieStatus = niks_te_doen;
				//mijnGrootsteHoek = mijnWielbelStartPlaats;

			}
		}
		break;
	}

	}
	if (mijnActieStatus != niks_te_doen) {
		Serial.print(myname);
		Serial.print(" herhalingen: ");
		Serial.print(mijnHerhalingen);
		Serial.print(" Mijn Huidige positie: ");
		Serial.print(mijnHuidigePositie);
		Serial.print(" Mijn Huidige status: ");
		Serial.println(getStatusName());
	}
}

void wiebel::startWiebel(uint8_t grootsteHoek, uint8_t kleinsteHoek,
		uint16_t tijdNaarGrootsteHoek, uint16_t tijdNaarKleinsteHoek,
		uint16_t wachtTijdGrootsteHoek, uint16_t wachtTijdKleinsteHoek,
		uint16_t startPlaats_pause_naar_grootst,
		uint16_t startPlaats_pause_naar_kleinst, uint16_t herhalingen) {

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
	mijnStartTijd = millis();
}

void wiebel::setPositie(uint8_t nieuwe) {
	mijnHuidigePositie = nieuwe;

	// zet je daar
	mijnServo.write(mijnHuidigePositie);
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
	case  midden_positie_naar_grootst:
	return "midden_positie_naar_grootst";
	case  helemaal_grootsts:
	return "helemaal_grootsts";
	case helemaal_kleinst:
		return "helemaal_kleinst";
	}
  return "unnamed state";
}
