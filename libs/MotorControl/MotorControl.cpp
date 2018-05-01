

/** Includes **/
/**************/
#include "MotorControl.h"
/** Constructor **/
/*****************/

MotorControl::MotorControl(Motor *left_motor, Motor *right_motor)
{
	m_left_pwm = 0;
	m_right_pwm = 0;
	m_left_motor = left_motor;
	m_right_motor = right_motor;
}






MotorControl::direction(Direction direction)
{
	switch (direction) {
	case LEFT_CROSSROADS:
		
		break;
	case LEFT_LINE:
		
		break;
	case RIGHT_LINE:

		break;
	case RIGHT_CROSSROADS:

		break;
	default:
		break;
	}
}

/** Private Methods **/
/*********************/