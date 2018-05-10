#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

/** Includes **/
/**************/

#include "Arduino.h"
#include <Motor.h>
#include <LineWatcher.h>


/** Defines **/
/*************/


typedef enum{
	GREEN,
	RED
}Color;

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
		LineWatcher* m_watcher;
		int16_t m_left_pwm;
		int16_t m_right_pwm;
		uint8_t m_sensitivity;
		int16_t m_direction;
		int16_t m_last_direction;
		int16_t m_pwm;
		int16_t m_pwm_crossroads;
		int16_t m_pwm_inverse_crossroads;
		float m_analog_sensitivity;
		uint8_t m_max_pwm_line_diffrence;
		uint32_t m_robot_starting_help_time;
		uint8_t m_robot_starting_help_test_number;
		uint16_t m_robot_starting_help_val[5];
		uint8_t* m_robotState;
		int16_t m_pwm_correction;
		uint8_t m_robot_starting_help_failed_nb;


	public:
		MotorControl(Motor *left_motor, Motor *right_motor, LineWatcher *watcher, uint8_t sensitivity, float analog_sensitivity, int16_t pwm_speed, int16_t pwm_crossroads_speed, int16_t pwm_crossroads_inverse_speed, uint8_t max_pwm_line_diffrence, uint8_t *robotState);
		//void direction(Direction direction);
		void analogDirection(Direction direction);
		void analogDirection(Direction direction, uint16_t rotation);
		void motorCrossroads(Direction direction);
		void update();
		void motorState(uint8_t robotState);
		void increasePwmSpeed();
		void stop();
		void StartingCurve(Color color);


};


#endif //MOTOR_CONTROL_H