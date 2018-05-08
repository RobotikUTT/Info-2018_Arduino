

/** Includes **/
/**************/
#include "MotorControl.h"
/** Constructor **/
/*****************/

MotorControl::MotorControl(Motor *left_motor, Motor *right_motor, uint8_t sensitivity, float analog_sensitivity, int16_t pwm_speed, int16_t pwm_crossroads_speed, int16_t pwm_crossroads_inverse_speed, uint8_t max_pwm_line_diffrence)
{
	m_pwm = pwm_speed;
	m_pwm_crossroads = pwm_crossroads_speed;
	m_pwm_inverse_crossroads = pwm_crossroads_inverse_speed;
	m_left_motor = left_motor;
	m_right_motor = right_motor;
	m_sensitivity = sensitivity;
	m_direction = CENTER;
	m_last_direction = CENTER;
	m_analog_sensitivity = analog_sensitivity;
	m_max_pwm_line_diffrence = max_pwm_line_diffrence;
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
void MotorControl::analogDirection(Direction direction)
{
	analogDirection(direction, 0);
}

void MotorControl::analogDirection(Direction direction, uint16_t rotation)
{
	switch (direction) {
	case LEFT_LINE:
		m_direction = -map(rotation*rotation*m_analog_sensitivity, 0, 65025, 5, 255);
		//m_direction = -rotation*m_analog_sensitivity;
		break;
	case RIGHT_LINE:
		m_direction = map(rotation*rotation*m_analog_sensitivity, 0, 65025, 5, 255);
		//m_direction = rotation*m_analog_sensitivity;
		break;
	default:
		m_direction = 0;
		break;
	}
	if(m_direction<-m_max_pwm_line_diffrence) {m_direction=-m_max_pwm_line_diffrence;}
	else if(m_direction>m_max_pwm_line_diffrence) {m_direction=m_max_pwm_line_diffrence;}

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

	/*if((abs(m_direction) < 5) && (abs(m_left_pwm - m_right_pwm) > 15))
	{
		m_left_pwm = (m_right_pwm - m_left_pwm)/2 + m_left_pwm;
		m_right_pwm = (m_left_pwm - m_right_pwm)/2 + m_right_pwm;
	}
	else */if(m_direction > 0)
	{
		m_left_pwm = m_pwm - 2*m_direction;
		m_right_pwm = m_pwm + m_direction;
	}
	else
	{
		m_left_pwm = m_pwm - 2*m_direction;
		m_right_pwm = m_pwm + m_direction;
	}

	//m_left_pwm = m_left_pwm + 3;//le moteur droit est plus lent
	

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
	
	m_last_direction = m_direction;
}

void MotorControl::motorCrossroads(Direction direction)
{
	switch (direction){
	case LEFT_CROSSROADS:
		m_left_pwm = -m_pwm_inverse_crossroads;
		m_right_pwm = m_pwm_crossroads;
		break;
	case RIGHT_CROSSROADS:
		m_left_pwm = m_pwm_crossroads;
		m_right_pwm = -m_pwm_inverse_crossroads;
		break;
	default:

		break;
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

void MotorControl::motorState(uint8_t robotState)
{
	if (robotState == 0)
	{
		m_left_pwm = 0;
		m_right_pwm = 0;
	}
}
/** Private Methods **/
/*********************/