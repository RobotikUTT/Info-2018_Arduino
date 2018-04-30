#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

/** Includes **/
/**************/

#include "Arduino.h"

/** Defines **/
/*************/
#define PHOTO_0_ERROR	1000
#define PHOTO_1_ERROR	1000
#define PHOTO_2_ERROR	1000
#define PHOTO_3_ERROR	1000
#define PHOTO_4_ERROR	1000

typedef enum{
	LINE_LOST,
	LINE_LEFT,
	LINE_RIGHT,
	LINE_CENTER
}LinePositionType;

#define NB_PHOTO	5

/** Class Descritpion **/

class lineFollower
{
	private:
		uint8_t m_photo_pins[NB_PHOTO];
		uint16_t m_photo_error[NB_PHOTO];
		uint16_t m_light_threshold;


	public:
		lineFollower(uint16_t light_threshold ,uint8_t photo_0_pin, uint8_t photo_1_pin, uint8_t photo_2_pin, uint8_t photo_3_pin, uint8_t photo_4_pin, uint16_t photo_0_error, uint16_t photo_1_error, uint16_t photo_2_error, uint16_t photo_3_error, uint16_t photo_4_error);//enregistrement des pins
		uint8_t photoState(uint8_t photo_num); //matière en dessous de la photo ; 0 : noir ; 1 : blanc
		uint16_t photoVal(uint8_t photo_num); //renvoit la valeur de la photo corrigée
		LinePositionType lineCase(); /*renvoit la situation de la ligne :
		0 : position de correspond a aucun cas
		1 : sur la ligne
		2 : robot a gauche de la ligne
		3 : robot a droite de la ligne*/
		void update();



};


#endif //LINE_FOLLOWER_H