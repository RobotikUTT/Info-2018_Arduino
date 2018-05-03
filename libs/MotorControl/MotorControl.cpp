

/** Includes **/
/**************/
#include "MotorControl.h"
/** Constructor **/
/*****************/

MotorControl::MotorControl(Motor *left_motor, Motor *right_motor, uint8_t sensitivity, float analog_sensitivity, uint8_t pwm_speed)
{
	m_pwm = pwm_speed;
	m_left_motor = left_motor;
	m_right_motor = right_motor;
	m_sensitivity = sensitivity;
	m_direction = CENTER;
	m_analog_sensitivity = analog_sensitivity;
}






/*void MotorControl::direction(Direction direction)
{
	switch (direction) {
	case LEFT_CROSSROADS:
		
		break;
	case LEFT_LINE:
		m_direction = -m_sensitivity;
		break;
	case RIGHT_LINE:
		m_direction = m_sensitivity;
		break;
	case RIGHT_CROSSROADS:

		break;
	default:
		m_direction = 0;
		break;
	}

	m_left_pwm = m_pwm - m_direction;
	m_right_pwm = m_pwm + m_direction;

	if(m_left_pwm > 255)
	{
		m_left_pwm = 255;
	}
	if(m_right_pwm > 255)
	{
		m_right_pwm = 255;
	}
	if(m_left_pwm < -255)
	{
		m_left_pwm = -255;
	}
	if(m_right_pwm < -255)
	{
		m_right_pwm = -255;
	}
}*/

void MotorControl::analogDirection(Direction direction, uint16_t rotation)
{
	switch (direction) {
	case LEFT_LINE:
		m_direction = -map(rotation*rotation*m_analog_sensitivity, 0, 65025, 0, 255);
		break;
	case RIGHT_LINE:
		m_direction = map(rotation*rotation*m_analog_sensitivity, 0, 65025, 0, 255);
		break;
	default:
		m_direction = 0;
		break;
	}
	if(m_direction<-40) {m_direction=-40;}
	else if(m_direction>40) {m_direction=40;}

	/*if(direction > 0)
	{
		m_left_pwm = m_pwm - 2*m_direction;

		m_left_pwm = m_left_pwm + m_direction;
		m_right_pwm = m_right_pwm + m_direction;

	}
	else
	{
		m_right_pwm = m_pwm - 2*m_direction;

		m_left_pwm = m_left_pwm + m_direction;
		m_right_pwm = m_right_pwm + m_direction;
	}*/
	m_left_pwm = m_pwm - m_direction;
	m_right_pwm = m_pwm-5 + m_direction;

	if(m_left_pwm > 255)
	{
		m_left_pwm = 255;
	}
	if(m_right_pwm > 255)
	{
		m_right_pwm = 255;
	}
	if(m_left_pwm < -255)
	{
		m_left_pwm = -255;
	}
	if(m_right_pwm < -255)
	{
		m_right_pwm = -255;
	}
}

void MotorControl::update()
{
	m_left_motor->setSpeed(m_left_pwm);
	m_right_motor->setSpeed(m_right_pwm);
	Serial.print(m_direction);
	Serial.print("  |  ");
	Serial.print(m_left_pwm);
	Serial.print("  |  ");
	Serial.print(m_right_pwm);
}
/** Private Methods **/
/*********************/