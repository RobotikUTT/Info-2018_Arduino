

/** Includes **/
/**************/
#include "MotorControl.h"
/** Constructor **/
/*****************/

MotorControl::MotorControl(Motor *left_motor, Motor *right_motor)
{
	m_left_motor = left_motor;
	m_right_motor = right_motor;
}

/** Private Methods **/
/*********************/