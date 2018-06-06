

/** Includes **/
/**************/
#include "HC_SR04.h"

/** Constructor **/
/*****************/

HC_SR04::HC_SR04(int trigger_pin, int echo_pin)
{
	m_trigger_pin = trigger_pin;
	m_echo_pin = echo_pin;

	pinMode(m_trigger_pin, OUTPUT);
	pinMode(m_echo_pin, INPUT);
	m_activated = true;
	
	init();
	m_measurement_start = 0;
	
}

/*** Destructor **/
/*****************/

HC_SR04::~HC_SR04()
{

}

/** Public Methods **/
/********************/
uint32_t HC_SR04::getDistance()
{
	// the distance is read, therefore it is 
	//not new.
	m_new_distance = false;
	return m_distance;
}

void HC_SR04::update()
{
	// if a trigger was sent we listen to a response,
	// otherwise we trigger a measure.
	if ( m_is_measuring )
	{
		listenToMeasure();		
	}
	else
	{
		triggerMeasure();
	}
}

void HC_SR04::enable()
{
	m_activated = true;
	m_measurement_start = micros();
}

void HC_SR04::disable()
{
	m_activated = false;
	// reset all flags
	m_is_measuring = false;
	m_waiting_falling_edge = false;
	m_new_distance = false;
	m_timed_out = false;
}

bool HC_SR04::hasNewDistance()
{
	return m_new_distance;
}

bool HC_SR04::enabled()
{
	return m_activated;
}

bool HC_SR04::timedOut()
{
	return m_timed_out;
}

/** Private Methods **/
/*********************/

void HC_SR04::triggerMeasure()
{	
	//if we send a trigger at least MEASURE_DELAY ms 
	// earlier we a new trigger
	if (m_activated && millis() - m_last_measure_time > MEASURE_DELAY)
	{
		// check if we are already sending a trigger.
		// if not, sending a trigger
		bool _is_triggered = digitalRead(m_trigger_pin) == HIGH;
		if ( _is_triggered )
		{
			//if the trigger is long enough, we stop it.
			if (micros() - m_trigger_micros > TRIGGER_PULSE)
		
			{
				digitalWrite(m_trigger_pin, LOW);
				//we keep a timestamp to know when was this
				// trigger
				m_last_measure_time = millis();
				// now we can listen to an answer
				m_is_measuring = true;
			}
		}
		else
		{
			digitalWrite(m_trigger_pin, HIGH);
			//timestamp of the begining of the trigger
			m_trigger_micros = micros();
		}
	}
}

void HC_SR04::listenToMeasure()
{	//if we have an answer
	if (digitalRead(m_echo_pin) == HIGH)
	{
		//if this is a rising edge 
		if (!m_waiting_falling_edge)
		{
			// timestamp of the rising edge
			m_measurement_start = micros();
			// we wait for the and of the answer
			m_waiting_falling_edge = true;
		}
	}
	// if we detect the end of the answer
	else if (m_waiting_falling_edge && digitalRead(m_echo_pin) == LOW)
	{
		//calculation of the time bewtween the end of
		// the trigger and right now
		uint32_t _travel_time = (micros() - m_measurement_start);
		 
		//computing the new distance (d = v * t)
		m_distance = _travel_time *SOUND_SPEED / 2.;
		
		// check if the new measure is out of a 
		// last_distance-centered window.
		// if it is, it is considered a new measure.
		/*m_new_distance = 
			m_distance > m_last_distance + NEW_MEASURE_HYSTERESIS;
		
		m_new_distance = 
			m_new_distance || m_distance < m_last_distance - NEW_MEASURE_HYSTERESIS;*/
		m_new_distance = true;
		// the new distance becomes the last one
		m_last_distance = m_distance;
		// measurement is finished
		m_is_measuring = false;
		// therefore we are not waiting for an answer anymore
		m_waiting_falling_edge = false;

		m_timed_out = false;
	}
	// if the answer times out
	else if (micros() - m_measurement_start > MEASURE_TIMEOUT)
	{
		//reseting values for next measure
		//Serial.println("to");
		m_measurement_start= micros();
		m_is_measuring = false;
		m_waiting_falling_edge = false;
		m_distance = 3000;
		m_last_distance = m_distance;
		m_activated = true;
		m_new_distance = false;
		m_timed_out = true;
	}
}

void HC_SR04::init()
{
	//inital values of the variables
	m_distance = 0x0;
	m_last_distance = 0x0;
	m_is_measuring = false;
	m_waiting_falling_edge = false;
	m_new_distance = false;
	m_activated = false;
	m_timed_out = false;
	m_trigger_micros = 0;
	m_last_measure_time = 0;
	

	digitalWrite(m_trigger_pin, LOW);
}