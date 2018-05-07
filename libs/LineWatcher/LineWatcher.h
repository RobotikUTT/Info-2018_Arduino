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
	LINE_CROSSROADS
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
		uint8_t photoBlackNb(LineColor color);
		LinePositionType lineSide();


};


#endif //LINE_WATCHER_H