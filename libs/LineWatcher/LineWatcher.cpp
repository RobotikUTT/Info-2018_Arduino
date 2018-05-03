

/** Includes **/
/**************/
#include "LineWatcher.h"
/** Constructor **/
/*****************/

LineWatcher::LineWatcher(uint16_t light_threshold_white, uint16_t light_threshold_black, uint8_t photo_0_pin, uint8_t photo_1_pin, uint8_t photo_2_pin, uint8_t photo_3_pin, uint8_t photo_4_pin, float photo_0_error_m, float photo_1_error_m, float photo_2_error_m, float photo_3_error_m, float photo_4_error_m, float photo_0_error_b, float photo_1_error_b, float photo_2_error_b, float photo_3_error_b, float photo_4_error_b)
{
	m_light_threshold_white = light_threshold_white;
	m_light_threshold_black = light_threshold_black;
	m_photo_pins[0] = photo_0_pin;
	m_photo_pins[1] = photo_1_pin;
	m_photo_pins[2] = photo_2_pin;
	m_photo_pins[3] = photo_3_pin;
	m_photo_pins[4] = photo_4_pin;
	m_photo_error_m[0] = photo_0_error_m;
	m_photo_error_m[1] = photo_1_error_m;
	m_photo_error_m[2] = photo_2_error_m;
	m_photo_error_m[3] = photo_3_error_m;
	m_photo_error_m[4] = photo_4_error_m;
	m_photo_error_b[0] = photo_0_error_b;
	m_photo_error_b[1] = photo_1_error_b;
	m_photo_error_b[2] = photo_2_error_b;
	m_photo_error_b[3] = photo_3_error_b;
	m_photo_error_b[4] = photo_4_error_b;
}

/** Public Methods **/
/********************/

/*void LineWatcher::update()
{
	switch (lineCase()) {
	case 2:
		//eteindre moteur droit
		break;
	case 3:
		//eteindre moteur gauche
		break;
	default:
		break;
	}
}*/

uint16_t LineWatcher::photoVal(uint8_t photo_num)
{
	return (uint16_t)analogRead(m_photo_pins[photo_num])*m_photo_error_m[photo_num]+m_photo_error_b[photo_num];
}

LineColor LineWatcher::photoState(uint8_t photo_num)
{
	if (photoVal(photo_num) >= m_light_threshold_white)
	{
		return WHITE;
	}
	else if (photoVal(photo_num) >= m_light_threshold_black)
	{
		return GREY;
	}
	else
	{
		return BLACK;
	}
}

LinePositionType LineWatcher::lineCase()
{
	LinePositionType linePos;
	if ((photoState(0) == WHITE) && (photoState(1) == BLACK) && (photoState(2) == WHITE) && (photoState(3) == BLACK) && (photoState(4) == WHITE))
	{
		linePos = LINE_CENTER; //sur la ligne
	}
	else if ((photoState(0) == WHITE) && (photoState(1) == WHITE) && /*(photoState(2) == BLACK) && */(photoState(3) == WHITE) && (photoState(4) == BLACK))
	{
		linePos = LINE_LEFT; //robot a gauche de la ligne
	}
	else if ((photoState(0) == BLACK) && (photoState(1) == WHITE) && /*(photoState(2) == BLACK) && */(photoState(3) == WHITE) &&(photoState(4) == WHITE))
	{
		linePos = LINE_RIGHT; //robot a droite de la ligne
	}
	else if ((photoState(0) == BLACK) && (photoState(1) == BLACK) && (photoState(2) == BLACK) && (photoState(3) == BLACK) &&(photoState(4) == BLACK))
	{
		linePos = LINE_CROSSROADS_1; //robot a intersection 3 croisements (choix entre droite et gauche)
	}
	else
	{
		linePos = LINE_LOST; //position ne correspond a aucun cas
	}
	return linePos;
}

LinePositionType LineWatcher::lineSide()
{
	if (photoVal(0) > photoVal(4))
	{
		return LINE_LEFT;//robot a gauche de la ligne
	}
	else
	{
		return LINE_RIGHT;//robot a droite de la ligne
	}
}

uint16_t LineWatcher::photoRead(uint8_t photo_num)
{
	return analogRead(m_photo_pins[photo_num]);
}
/** Private Methods **/
/*********************/