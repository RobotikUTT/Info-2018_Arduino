#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

/** Includes **/
/**************/

#include "Arduino.h"
#include <Motor.h>

/** Defines **/
/*************/

typedef enum{
	LEFT_CROSSROADS,
	LEFT_LINE,
	CENTER,
	RIGHT_LINE,
	RIGHT_CROSSROADS
}Direction;

/** Class Descritpion **/

class MotorControl
{
	private:
		Motor* m_left_motor;
		Motor* m_right_motor;
		int16_t m_left_pwm;
		int16_t m_right_pwm;


	public:
		MotorControl(Motor *left_motor, Motor *right_motor);
		direction(Direction direction); //-2:virage croisement gauche |1:tourner à gauche pour se remettre sur la ligne |0:continuer tout droit |1 et 2: pareil à droite

};


#endif //MOTOR_CONTROL_H