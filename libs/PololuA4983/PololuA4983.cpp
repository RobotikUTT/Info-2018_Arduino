

/** Includes **/
/**************/
#include "PololuA4983.h"

/** Constructor **/
/*****************/

PololuA4983::PololuA4983(int step_pin, int dir_pin, int en_pin)
{
	m_dir_pin  = dir_pin;
	m_step_pin = step_pin;
	m_en_pin = en_pin;
	m_remaining_steps = 0;
	m_last_step_time = elapsedTime();

	pinMode(m_step_pin, OUTPUT);
	pinMode(m_dir_pin,  OUTPUT);
	pinMode(m_en_pin,   OUTPUT);

	digitalWrite(m_step_pin, LOW);
	digitalWrite(m_dir_pin,  LOW);
	enable();
	
}

PololuA4983::PololuA4983(int step_pin, int dir_pin)
{
	PololuA4983(step_pin,dir_pin,0);
}

/*** Destructor **/
/*****************/
PololuA4983::~PololuA4983()
{

}

/** Public Methods **/
/********************/

void PololuA4983::update()
{
	if (m_remaining_steps != 0)
	{
		if ( m_remaining_steps > 0)
		{
			digitalWrite(m_dir_pin, HIGH);
		}
		else
		{
			digitalWrite(m_dir_pin, LOW);
		}

		if (elapsedTime() - m_last_step_time > MIN_DELAY)
		{
			if ( digitalRead(m_step_pin) == HIGH )
			{
				digitalWrite(m_step_pin, LOW);
				if ( m_remaining_steps > 0)
				{
					m_remaining_steps --;
				}
				else
				{
					m_remaining_steps ++;
				}

			}
			else
			{
				digitalWrite(m_step_pin, HIGH);
			}
			m_last_step_time = elapsedTime();
		}


	}
}

void PololuA4983::moveStep(uint16_t nb_steps, bool dir)
{
	
	if (dir)
	{
		
		m_remaining_steps += nb_steps;
	}
	else
	{
		
		m_remaining_steps -= nb_steps;
	}
	
}

void PololuA4983::moveRevolution(uint16_t nb_rev, bool dir)
{
	moveStep(STEP_PER_REVOLUTION * nb_rev, dir);
}

void PololuA4983::enable()
{
	if (m_en_pin > 0)
	{
		digitalWrite(m_en_pin, ENABLE);
	}
}

void PololuA4983::disable()
{
	if (m_en_pin > 0)
	{
		digitalWrite(m_en_pin, DISABLE);
	}
}

void PololuA4983::stop()
{
	m_remaining_steps = 0;
}

/** Private Methods **/
/*********************/

uint32_t PololuA4983::elapsedTime()
{
	return micros();
}