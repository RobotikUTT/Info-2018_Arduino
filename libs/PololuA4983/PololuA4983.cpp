

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
	m_flagFifoEmpty = true;
	
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
		m_flagFifoEmpty = false;
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
	else
	{
		if (m_goalsList.count() > 0)
		{
			m_remaining_steps = m_goalsList.pop();
		}
		else
		{
			m_flagFifoEmpty = true;
		}
	}
}

void PololuA4983::moveStep(int16_t nb_steps)
{
	m_goalsList.push(nb_steps);
}

void PololuA4983::moveRevolution(int16_t nb_rev)
{
	moveStep(STEP_PER_REVOLUTION * nb_rev);
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

bool PololuA4983::fifoEmpty()
{
	return m_flagFifoEmpty;
}

/** Private Methods **/
/*********************/

uint32_t PololuA4983::elapsedTime()
{
	return micros();
}