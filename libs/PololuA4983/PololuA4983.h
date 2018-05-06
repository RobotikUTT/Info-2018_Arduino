#ifndef POLOLU_A4983_H
#define POLOLU_A4983_H

#define ENABLE LOW
#define DISABLE HIGH
#define MIN_DELAY 500 //us //350
#define STEP_PER_REVOLUTION 200

/** Includes **/
/**************/

#include "Arduino.h"
#include "QueueList.h"

/** Defines **/
/*************/


/** Class Descritpion **/

class PololuA4983
{
	private:
		uint8_t m_dir_pin;
		uint8_t m_step_pin;
		uint8_t m_en_pin;
		uint32_t m_last_step_time;
		int16_t m_remaining_steps;
		uint32_t elapsedTime();
		QueueList<int16_t> m_goalsList;

		bool m_flagFifoEmpty;



	public:
		PololuA4983(int step_pin, int dir_pin, int en_pin);
		PololuA4983(int step_pin, int dir_pin);
		~PololuA4983();
		void moveStep(int16_t nb_steps);
		void moveRevolution(int16_t nb_rev);
		
		void enable();
		void disable();
		void update();
		void stop();
		bool fifoEmpty();
		int16_t remainingSteps();

		

};


#endif //POLOLU_A4983_H