

/** Includes **/
/**************/
#include "Pliers.h"

/** Constructor **/
/*****************/
Pliers::Pliers(PololuA4983* stepper, int limit_switch_pin)
{
	m_stepper = stepper;
	m_limit_switch_pin = limit_switch_pin;

	pinMode(m_limit_switch_pin, INPUT);
	reset();
	m_current_level = 1;
}

Pliers::~Pliers()
{
	delete m_stepper;
}

/** Public Methods **/
/********************/

void Pliers::update()
{
	if (digitalRead(m_limit_switch_pin) == LIMIT_SWITCH_PRESSED)
	{
		m_stepper->stop();
	}
	m_stepper->update();
}

void Pliers::reset()
{
	m_stepper->moveStep((MAX_LEVEL+1)*STEP_PER_LEVEL, DOWN);
	m_current_level = 1;
}

void Pliers::setLevel(uint8_t level)
{
	bool dir;
	if (level < MIN_LEVEL)
	{
		level = MIN_LEVEL;
	}
	if (level > MAX_LEVEL)
	{
		level = MAX_LEVEL;
	}
	if (level > m_current_level)
	{
		dir = UP;
	}
	else
	{
		dir = DOWN;
	}
	m_stepper->moveStep(level*STEP_PER_LEVEL, dir);
}

void Pliers::open()
{

}

void Pliers::close()
{

}
/** Private Methods **/
/*********************/