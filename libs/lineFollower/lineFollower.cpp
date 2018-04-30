

/** Includes **/
/**************/
#include "lineFollower.h"
/** Constructor **/
/*****************/

lineFollower::lineFollower(uint16_t light_threshold ,uint8_t photo_0_pin, uint8_t photo_1_pin, uint8_t photo_2_pin, uint8_t photo_3_pin, uint8_t photo_4_pin, uint16_t photo_0_error, uint16_t photo_1_error, uint16_t photo_2_error, uint16_t photo_3_error, uint16_t photo_4_error)
{
	m_light_threshold = light_threshold;
	m_photo_pins[0] = photo_0_pin;
	m_photo_pins[1] = photo_1_pin;
	m_photo_pins[2] = photo_2_pin;
	m_photo_pins[3] = photo_3_pin;
	m_photo_pins[4] = photo_4_pin;
	m_photo_error[0] = photo_0_error;
	m_photo_error[1] = photo_1_error;
	m_photo_error[2] = photo_2_error;
	m_photo_error[3] = photo_3_error;
	m_photo_error[4] = photo_4_error;
}

/** Public Methods **/
/********************/

void lineFollower::update()
{
	/*switch (lineCase()) {
	case 2:
		//eteindre moteur droit
		break;
	case 3:
		//eteindre moteur gauche
		break;
	default:
		break;
	}*/
}

uint16_t lineFollower::photoVal(uint8_t photo_num)
{
	return analogRead(m_photo_pins[photo_num])*m_photo_error[photo_num]/1000;
}

uint8_t lineFollower::photoState(uint8_t photo_num)
{
	if (photoVal(photo_num) >= m_light_threshold)
	{
		return 1; //blanc
	}
	else
	{
		return 0; //noir
	}
}

LinePositionType lineFollower::lineCase()
{
	LinePositionType linePos;
	if ((photoState(1) == 0) && (photoState(2) == 1) && (photoState(3) == 0))
	{
		linePos = LINE_CENTER; //sur la ligne
	}
	else if ((photoState(2) == 0) && (photoState(3) == 1) && (photoState(4) == 0))
	{
		linePos = LINE_LEFT; //robot a gauche de la ligne
	}
	else if ((photoState(0) == 0) && (photoState(1) == 1) && (photoState(2) == 0))
	{
		linePos = LINE_RIGHT; //robot a droite de la ligne
	}
	else
	{
		linePos = LINE_LOST; //position ne correspond a aucun cas
	}
	return linePos;
}
/** Private Methods **/
/*********************/