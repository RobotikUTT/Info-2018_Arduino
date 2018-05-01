#ifndef MOTOR_H
#define MOTOR_H

/** Includes **/
/**************/

#include "Arduino.h"

/** Defines **/
/*************/


/** Class Descritpion **/

class Motor
{
	private:
		uint8_t m_enable_pin;
		uint8_t m_dir_pin;
		uint8_t m_brake_pin;
		uint8_t m_foreward_sense;
		uint8_t m_backward_sense;



	public:
		Motor(uint8_t enable_pin, uint8_t dir_pin, uint8_t brake_pin, uint8_t foreward_sense);
		Motor(uint8_t enable_pin, uint8_t dir_pin, uint8_t foreward_sense);
		void setSpeed(int pwm);

		void stop();
		void motor_brake();


};


#endif //MOTOR_H