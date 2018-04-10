#ifndef PLIERS_H
#define PLIERS_H

/** Includes **/
/**************/

#include "Arduino.h"
#include "PololuA4983.h"
// #include "AX12.h"

/** Defines **/
/*************/
#define MIN_LEVEL 1
#define MAX_LEVEL 4
#define UP 	true
#define DOWN false
#define STEP_PER_LEVEL STEP_PER_REVOLUTION*2
#define LIMIT_SWITCH_PRESSED	HIGH

/** Class Descritpion **/

class Pliers
{
	private:
		PololuA4983* m_stepper;
		int16_t m_limit_switch_pin;
		uint8_t m_current_level;
		// AX12 m_ax12;

	public:
		Pliers(PololuA4983* stepper,int limit_switch_pin);
		~Pliers();
		void update();
		void setLevel(uint8_t level);
		void open();
		void close();
		void reset();


};


#endif //PLIERS_H