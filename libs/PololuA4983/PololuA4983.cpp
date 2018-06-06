

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

	//pinMode(m_step_pin, OUTPUT);
	//pinMode(m_dir_pin,  OUTPUT);
	//pinMode(m_en_pin,   OUTPUT);

	pinMode(STEPPER_DIR, OUTPUT);
    pinMode(STEPPER_STEP, OUTPUT);

	digitalWrite(STEPPER_STEP, LOW);
	digitalWrite(STEPPER_DIR,  LOW);
	enable();
	m_flagFifoEmpty = true;
	m_set_high = false;
	
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
			digitalWrite(STEPPER_DIR, LOW);
		}
		else
		{
			digitalWrite(STEPPER_DIR, HIGH);
		}

		if (elapsedTime() - m_last_step_time > MIN_DELAY)
		{
			if ( m_set_high )
			{
				//Serial.println("low");
				m_set_high = false;
				digitalWrite(STEPPER_STEP, LOW);
				if ( m_remaining_steps > 0)
				{
					m_remaining_steps --;
				}
				else
				{
					m_remaining_steps ++;
				}
				// Serial.print("remaining steps:");
				// Serial.println(m_remaining_steps);

			}
			else
			{
				//Serial.println("high");
				m_set_high = true;
				digitalWrite(STEPPER_STEP, HIGH);
			}
			m_last_step_time = elapsedTime();
		}


	}
	else
	{
		if (m_goalsList.count() > 0)
		{
			//Serial.println("new goal");
			m_remaining_steps = m_goalsList.pop();
			m_flagFifoEmpty = false;
			
			
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

int16_t PololuA4983::remainingSteps()
{
	return m_remaining_steps;
}
void PololuA4983::init()
{
	
}
/** Private Methods **/
/*********************/

uint32_t PololuA4983::elapsedTime()
{
	return micros();
}