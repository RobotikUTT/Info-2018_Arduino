#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

/** Includes **/
/**************/

#include "Arduino.h"
#include <LineWatcher.h>
#include <Motor.h>
#include <MotorControl.h>

/** Defines **/
/*************/
typedef enum{
	LEFT,
	FORWARD,
	RIGHT,
	BACKWARD,
	UNKNOWN,
	FORWARDCROSSROADS
}CrossroadsDirection;

/** Class Descritpion **/

class LineFollower
{
	private:
		LineWatcher* m_watcher;
		MotorControl* m_motor;
		uint16_t m_max_photo_luminosity;
		uint8_t m_crossroads_nb;
		uint8_t crossroadsEvent;
		CrossroadsDirection m_next_crossroads;
		Color* m_color;
		

	public:
		LineFollower(LineWatcher *watcher, MotorControl *motor, uint16_t max_photo_luminosity, Color *color);
		void NextCrossroads(CrossroadsDirection direction);
		void update(uint8_t &robotState);
		CrossroadsDirection m_direction;
		uint8_t getCrossroadsNb();
		CrossroadsDirection getNextCrossroads();
		void addNextCrossroads(CrossroadsDirection next_crossroads);

};


#endif //LINE_FOLLOWER_H