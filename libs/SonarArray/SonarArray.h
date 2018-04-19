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
		std::vector<HC_SR04> 	m_sonarVector;
		std::vector<uint16_t>	m_distances;
		std::vector<bool> 		m_activatedSonars;
		uint8_t					m_sonar;

	public:
		SonarArray(uint8_t nbSonars, ...);
		//SonarArray();
		~SonarArray();

		void update();
		std::vector<uint16_t> getDistances();
		void addSonar(int trig_pin, int echo_pin);
		uint8_t getNbSonars();
		void disableSonar(uint8_t sonarNb);
		void enableSonar(uint8_t sonarNb);
		void enableAll();
		void disableAll();





		

};


#endif //SONAR_ARRAY_H