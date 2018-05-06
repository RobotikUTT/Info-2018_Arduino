#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

/** Includes **/
/**************/

#include "Arduino.h"

/** Defines **/
/*************/


/** Class Descritpion **/

class MotorControl
{
	private:
		uint8_t m_enable_pin;
		uint8_t m_dir_pin;
		uint8_t m_brake_pin;
		uint8_t m_foreward_sense;
		uint8_t m_backward_sense;



	public:
		MotorControl(uint8_t enable_pin, uint8_t dir_pin, uint8_t brake_pin, uint8_t foreward_sense);
		MotorControl(uint8_t enable_pin, uint8_t dir_pin, uint8_t foreward_sense);
		void setSpeed(int pwm);

		void stop();
		void brake();


};


#endif //MOTOR_CONTROL_H