#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

/** Includes **/
/**************/

#include "Arduino.h"

/** Defines **/
/*************/
//#define PHOTO_0_ERROR
#define NB_PHOTO	5

/** Class Descritpion **/

class lineFollower
{
	private:
		uint8_t m_photo_pins[NB_PHOTO];



	public:
		lineFollower(uint8_t photo0pin, uint8_t photo1pin, uint8_t photo2pin, uint8_t photo3pin, uint8_t photo4pin);//enregistrement des pins
		//uint8_t photoState(uint8_t photo_pin); //matière en dessous de la photo ; 0 : noir ; 1 : blanc
		//uint16_t photoVal(uint8_t photo_pin); //renvoit la valeur de la photo corrigée



};


#endif //LINE_FOLLOWER_H