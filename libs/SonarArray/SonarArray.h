#ifndef SONAR_ARRAY_H
#define SONAR_ARRAY_H

/** Includes **/
/**************/

#include "Arduino.h"
#include "HC_SR04.h"
#include <StandardCplusplus.h>
#include <vector>
#include <map>
#include <iterator>
/** Defines **/
/*************/


/** Class Descritpion **/

class SonarArray
{
	private:
		// vector that contains every sonar object
		std::vector<HC_SR04> 	m_sonarVector;
		// vector containing every last distance 
		// measured by the sonars
		std::vector<uint16_t>	m_distances;
		// Contains booleans to provide activation 
		// and deactivation of specific sonars
		std::vector<bool> 		m_activatedSonars;
		// counter used to interact with the right sonar
		uint8_t					m_array_counter;

	public:
		// in order to use thihs class, you must 
		// call this constructor with the number of sonar
		// you want and feed the trigger and echo pins of
		// each sonar, in that order 
		// (e.g. SonarArray(2, trig1; echo1,trig2,echo2) )
		SonarArray(uint8_t nbSonars, ...);
		~SonarArray();

		//function to call in your main loop
		void update();
		// returns the array with every distance
		std::vector<uint16_t> getDistances();
		// adds a sonar to the array
		void addSonar(int trig_pin, int echo_pin);
		// return the number of sonars in the array
		uint8_t getNbSonars();
		// disable a specific sonar. 
		// the number corresponds to the
		// n-th sonar added. e.g. the sonar
		// 0 is the first sonar added.
		void disableSonar(uint8_t sonarNb);
		// enable a specific sonar
		void enableSonar(uint8_t sonarNb);
		// enable all sonars
		void enableAll();
		// disable all sonars
		void disableAll();
};


#endif //SONAR_ARRAY_H