

/** Includes **/
/**************/
#include "MotorControl.h"
/** Constructor **/
/*****************/

MotorControl::MotorControl(Motor *left_motor, Motor *right_motor, LineWatcher *watcher, uint8_t sensitivity, float analog_sensitivity, int16_t pwm_speed, int16_t pwm_crossroads_speed, int16_t pwm_crossroads_inverse_speed, uint8_t max_pwm_line_diffrence, uint8_t *robotState)
{
	m_pwm = pwm_speed;
	m_pwm_crossroads = pwm_crossroads_speed;
	m_pwm_inverse_crossroads = pwm_crossroads_inverse_speed;
	m_left_motor = left_motor;
	m_right_motor = right_motor;
	m_watcher = watcher;
	m_sensitivity = sensitivity;
	m_direction = CENTER;
	m_last_direction = CENTER;
	m_analog_sensitivity = analog_sensitivity;
	m_max_pwm_line_diffrence = max_pwm_line_diffrence;
	m_robot_starting_help_time = millis();
	m_robot_starting_help_test_number = 0;
	m_robotState = robotState;
	//m_pwm_correction = 0;
	m_robot_starting_help_failed_nb = 0;
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
		m_direction = -map(rotation*rotation*rotation*m_analog_sensitivity, 0, 16581375, 0, 255);
		//m_direction = -rotation*m_analog_sensitivity;
		break;
	case RIGHT_LINE:
		m_direction = map(rotation*rotation*rotation*m_analog_sensitivity, 0, 16581375, 0, 255);
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
		m_left_pwm = m_pwm - 1*m_direction;
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

	m_left_motor->setSpeed(m_left_pwm); //a laisser ?
	m_right_motor->setSpeed(m_right_pwm);
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
	m_left_motor->setSpeed(m_left_pwm);
	m_right_motor->setSpeed(m_right_pwm);

}

void MotorControl::update()
{
	//augmentation de la vitesse des moteurs si le robot ne demarre pas :
	if (*m_robotState == 1)
	{
		analogDirection(CENTER);
		if ((m_robot_starting_help_test_number == 0) && (millis()-m_robot_starting_help_time > 50))
		{
			m_robot_starting_help_test_number++;
			m_robot_starting_help_time = millis();
			for(int i = 0; i < 5; i++)
			{
				m_robot_starting_help_val[i] = m_watcher->photoVal(i);
			}
		}
		else if (millis()-m_robot_starting_help_time > 50)
		{
			uint8_t localVar = 0;
	
			for (int i = 0; i < 5; i++)
			{
				if (abs(m_robot_starting_help_val[i] - m_watcher->photoVal(i)) < 15)
				{
					localVar++;
					//Serial.print("  |localVar:  ");
					//Serial.print(m_robot_starting_help_test_number);
				}
			}
	
			if (localVar >= 4)
			{
				m_robot_starting_help_test_number++;
			}
			else
			{
				m_robot_starting_help_test_number = 0;
				m_robot_starting_help_failed_nb++;
			}
	
			m_robot_starting_help_time = millis();
		}
	
		if (m_robot_starting_help_test_number >= 3)
		{
			increasePwmSpeed();
			m_robot_starting_help_test_number = 0;
		}
	
		//Serial.print("  |start:  ");
		//Serial.print(m_robot_starting_help_test_number);

		if (m_robot_starting_help_failed_nb > 7)
		{
			*m_robotState = *m_robotState + 1;
			m_pwm = m_pwm + 28;
			m_pwm_crossroads = m_pwm_crossroads + 35;
			/*m_left_motor->setSpeed(255);
			m_right_motor->setSpeed(255);
			delay(400);*/

		}
	}



	//reste de la fonction
	m_left_pwm = m_left_pwm*1.6;
	if (m_left_pwm > 255)
	{
		m_left_pwm = 255;
	}
	else if (m_left_pwm < -255)
	{
		m_left_pwm = -255;
	}
	m_left_motor->setSpeed(m_left_pwm);
	m_right_motor->setSpeed(m_right_pwm);
	Serial.print(m_direction);
	Serial.print("  |pwm: ");
	Serial.print(m_left_pwm);
	Serial.print("  |  ");
	Serial.print(m_right_pwm);
}

void MotorControl::motorState(uint8_t robotState)
{
	if (robotState == 0)
	{
		stop();
	}
}

void MotorControl::increasePwmSpeed()
{
	//m_pwm_correction = m_pwm_correction + 8;
	m_pwm = m_pwm + 4;
	m_pwm_crossroads = m_pwm_crossroads + 4;
}

void MotorControl::stop()
{
	m_left_pwm = 0;
	m_right_pwm = 0;
	m_left_motor->setSpeed(m_left_pwm);
	m_right_motor->setSpeed(m_right_pwm);
}


void MotorControl::StartingCurve(Color color)
{
	if (color == GREEN)
	{
		m_left_pwm = m_pwm*1.6 - 160;
		m_right_pwm = m_pwm + 70;
	}
	else
	{
		m_left_pwm = m_pwm*1.6 + 20;
		m_right_pwm = m_pwm - 50;
	}

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

	m_left_motor->setSpeed(m_left_pwm);
	m_right_motor->setSpeed(m_right_pwm);
}

/** Private Methods **/
/*********************/