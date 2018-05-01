#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

/** Includes **/
/**************/

#include "Arduino.h"
#include <Motor.h>

/** Defines **/
/*************/


/** Class Descritpion **/

class MotorControl
{
	private:
		Motor* m_left_motor;
		Motor* m_right_motor;


	public:
		MotorControl(Motor *left_motor, Motor *right_motor);

};


#endif //MOTOR_CONTROL_H