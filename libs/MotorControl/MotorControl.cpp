

/** Includes **/
/**************/
#include "MotorControl.h"
/** Constructor **/
/*****************/

MotorControl::MotorControl(uint8_t enable_pin, uint8_t dir_pin, uint8_t brake_pin, uint8_t foreward_sense)
{
	m_enable_pin = enable_pin;
	m_dir_pin = dir_pin;
	m_brake_pin = brake_pin;
	m_foreward_sense = foreward_sense;
	if (m_foreward_sense == HIGH)
	{
		m_backward_sense = LOW;
	}
	else
	{
		m_backward_sense = HIGH;
	}

	pinMode(m_enable_pin, OUTPUT);
	pinMode(m_dir_pin, OUTPUT);
	pinMode(m_brake_pin, OUTPUT);

	digitalWrite(m_enable_pin,LOW);
	digitalWrite(m_brake_pin,LOW);
	digitalWrite(m_dir_pin,LOW);
}

MotorControl::MotorControl(uint8_t enable_pin, uint8_t dir_pin, uint8_t foreward_sense)
{
	MotorControl(enable_pin,dir_pin,0,foreward_sense);
}

/** Public Methods **/
/********************/
void MotorControl::setSpeed(int pwm)
{
	if (pwm < 0)
	{
		digitalWrite(m_dir_pin, m_backward_sense);
		if ( m_brake_pin >0 );
		{
			digitalWrite(m_brake_pin, HIGH);
		}
	}
	else
	{
		digitalWrite(m_dir_pin, m_foreward_sense);
		if ( m_brake_pin >0 )
		{
			digitalWrite(m_brake_pin, LOW);
		}
		
	}
	//Serial.println(pwm);
	analogWrite(m_enable_pin, abs(pwm));
}

void MotorControl::stop()
{
	analogWrite(m_enable_pin, 0);
}

void MotorControl::motor_brake()
{
	stop();
	digitalWrite(m_dir_pin, HIGH);

	if ( m_brake_pin >0 );
		{
			digitalWrite(m_brake_pin, HIGH);
		}
}
/** Private Methods **/
/*********************/