

/** Includes **/
/**************/
#include "lineFollower.h"
/** Constructor **/
/*****************/

lineFollower::lineFollower(uint8_t photo_0_pin, uint8_t photo_1_pin, uint8_t photo_2_pin, uint8_t photo_3_pin, uint8_t photo_4_pin)
{
	m_photo_pins = {photo_0_pin, photo_1_pin, photo_2_pin, photo_3_pin, photo_4_pin};
}


/** Private Methods **/
/*********************/