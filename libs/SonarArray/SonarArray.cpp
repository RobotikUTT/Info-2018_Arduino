/** Includes **/
/**************/
#include "SonarArray.h"

/** Constructor **/
/*****************/
SonarArray::SonarArray(uint8_t nbSonars, ...)
{
	int _trig_pin, _echo_pin;
	va_list argv;
	va_start(argv,nbSonars);


	for (uint8_t i = 0; i < nbSonars; i ++)
	{
		_trig_pin = va_arg(argv, int);
		_echo_pin = va_arg(argv, int);
		HC_SR04 sonar(_trig_pin,_echo_pin);
		m_sonarVector.push_back(sonar);
		
		m_activatedSonars.push_back(true);
		
		m_distances.push_back(sonar.getDistance() );

	}
	
	m_array_counter = 0;
	va_end(argv);
}

/*** Destructor **/
/*****************/

SonarArray::~SonarArray()
{
	m_sonarVector.clear();
	m_activatedSonars.clear();
	m_distances.clear();
}


/** Public Methods **/
/********************/

void SonarArray::update()
{
	
	uint8_t i = m_array_counter % m_sonarVector.size();
	HC_SR04* sonar = &m_sonarVector[i];
	if ( m_activatedSonars[i] )
	{
		if ( !sonar->enabled() )
		{
			sonar->enable();
		}

		sonar->update();
		if (sonar->hasNewDistance() || sonar->timedOut())
		{
			m_distances[i] = sonar->getDistance();
			sonar->disable();
			m_sonarVector[ (i+1)%m_sonarVector.size() ].enable();
			m_array_counter++;
		}
	}
	else
	{
		m_array_counter++;
	}

	
}

std::vector<uint16_t> SonarArray::getDistances()
{
	return m_distances;
}

void SonarArray::addSonar(int trig_pin, int echo_pin)
{
	HC_SR04 sonar(trig_pin,echo_pin);
	m_sonarVector.push_back(sonar);
	m_activatedSonars.push_back(true);
}

uint8_t SonarArray::getNbSonars()
{
	return m_sonarVector.size();
}

void SonarArray::disableSonar(uint8_t sonarNb)
{
	if ( sonarNb < getNbSonars() && sonarNb >= 0)
	{
		m_activatedSonars[sonarNb] = false;
	}
}

void SonarArray::enableSonar(uint8_t sonarNb)
{
	if ( sonarNb < getNbSonars() && sonarNb >= 0)
	{
		m_activatedSonars[sonarNb] = true;
	}
}

void SonarArray::enableAll()
{
	for (uint8_t i = 0; i < getNbSonars(); i++)
	{
		m_activatedSonars[i] = true;
	}
}

void SonarArray::disableAll()
{
	for (uint8_t i = 0; i < getNbSonars(); i++)
	{
		m_activatedSonars[i] = false;
	}
}
/** Private Methods **/
/*********************/