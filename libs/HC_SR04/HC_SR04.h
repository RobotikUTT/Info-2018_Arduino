/**************************\
This class controls an HC SR04 sonar. 
It uses a non-blocking process, very 
useful to build reactive robots. 
The measure process is disabled by 
default. You must first call the enable()
function and the call the update() 
function in your main loop.
every measure is in millimeters.

Author: Apache
/**************************/

#ifndef HC_SR04_H
#define HC_SR04_H

/** Includes **/
/**************/

#include "Arduino.h"

/** Defines **/
/*************/
#define TRIGGER_PULSE 	10 //us
#define MEASURE_DELAY	(uint32_t)60	//ms
#define SOUND_SPEED		0.3432 //mm/us
#define TIME_PROPORTION 58
#define MEASURE_TIMEOUT (uint32_t)MEASURE_DELAY*1000*5
#define NEW_MEASURE_HYSTERESIS 3

/** Class Descritpion **/

class HC_SR04
{
	private:
		uint8_t 	m_trigger_pin;
		uint8_t 	m_echo_pin;
		uint32_t 	m_distance; //measured distance
		uint32_t 	m_last_distance; //last measured distance
		//this flag is true when a mesure order is sent 
		//to the sonar
		bool 		m_is_measuring; 
		//this flag is true when a rising edge
		//is detected on the echo pin.
		bool		m_waiting_falling_edge;
		//flag is true when the new measure
		//is not equal to the last measure.
		//it is set to false when the new 
		//distance  is read.
		bool 		m_new_distance;
		//flag to enable the measure processing.
		//usueful when you have a blocking process
		//or many sonars
		bool		m_activated;

		// timestamp of the moment the trigger
		// pin was set to 1
		uint32_t 	m_trigger_micros;
		//timestamp of the last rising edge 
		//on the echo pin
		uint32_t 	m_measurement_start;
		// timestamp of the last measure
		// order. It is used to allow a delay
		// between the measurements.
		uint32_t 	m_last_measure_time;

		// sends an measurement order
		// to the sonar
		void triggerMeasure();
		// listens to the to the echo pin
		// and calculates the distance when
		// an anwser is received
		void listenToMeasure();
		// initiate the members values.
		void init();
		
	public:
		HC_SR04(int trigger_pin, int echo_pin);
		~HC_SR04();

		// returns the distance measured
		uint32_t 	getDistance();
		// updates the mesurement process
		void 		update();
		// true if the new measure is 
		//different from the last one
		bool		hasNewDistance();
		//enables the process
		void 		enable();
		//disables the process
		void 		disable();
		//true if the process is enabled
		bool		enabled();

};


#endif //HC_SR04_H