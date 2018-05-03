#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

/** Includes **/
/**************/

#include "Arduino.h"
#include <Motor.h>

/** Defines **/
/*************/

typedef enum{
	LEFT_CROSSROADS,//pour intersection
	LEFT_LINE,//pour tourner quand on quitte la ligne
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
		uint8_t m_sensitivity;
		int16_t m_direction;
		int16_t m_pwm;
		float m_analog_sensitivity;


	public:
		MotorControl(Motor *left_motor, Motor *right_motor, uint8_t sensitivity, float analog_sensitivity, uint8_t pwm_speed);
		//void direction(Direction direction);
		void analogDirection(Direction direction, uint16_t rotation);
		void update();

};


#endif //MOTOR_CONTROL_H