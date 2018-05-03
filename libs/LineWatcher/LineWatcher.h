#ifndef LINE_WATCHER_H
#define LINE_WATCHER_H

/** Includes **/
/**************/

#include "Arduino.h"

/** Defines **/
/*************/

typedef enum{
	LINE_LOST,
	LINE_LEFT,
	LINE_RIGHT,
	LINE_CENTER,
	LINE_CROSSROADS_1
}LinePositionType;

typedef enum{
	WHITE,
	GREY,
	BLACK
}LineColor;

#define NB_PHOTO	5

/** Class Descritpion **/

class LineWatcher
{
	private:
		uint8_t m_photo_pins[NB_PHOTO];
		float m_photo_error_m[NB_PHOTO];
		float m_photo_error_b[NB_PHOTO];
		uint16_t m_light_threshold_white;
		uint16_t m_light_threshold_black;


	public:
		LineWatcher(uint16_t light_threshold_white, uint16_t light_threshold_black ,uint8_t photo_0_pin, uint8_t photo_1_pin, uint8_t photo_2_pin, uint8_t photo_3_pin, uint8_t photo_4_pin, float photo_0_error_m, float photo_1_error_m, float photo_2_error_m, float photo_3_error_m, float photo_4_error_m, float photo_0_error_b, float photo_1_error_b, float photo_2_error_b, float photo_3_error_b, float photo_4_error_b);
		LineColor photoState(uint8_t photo_num); //matière en dessous de la photo ; 0 : noir ; 1 : blanc
		uint16_t photoVal(uint8_t photo_num); //renvoit la valeur de la photo corrigée
		uint16_t photoRead(uint8_t photo_num); //revoit la valeur de la photo non corrigée
		LinePositionType lineCase(); /*renvoit la situation de la ligne :
		0 : position de correspond a aucun cas
		1 : sur la ligne
		2 : robot a gauche de la ligne
		3 : robot a droite de la ligne*/
		LinePositionType lineSide();


};


#endif //LINE_WATCHER_H