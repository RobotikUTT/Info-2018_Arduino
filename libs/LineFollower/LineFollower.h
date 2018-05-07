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
	UNKNOWN
}CrossroadsDirection;

/** Class Descritpion **/

class LineFollower
{
	private:
		LineWatcher* m_watcher;
		MotorControl* m_motor;

	public:
		LineFollower(LineWatcher *watcher, MotorControl *motor);
		void NextCrossroads(CrossroadsDirection direction);
		void update(uint8_t robotState);
		CrossroadsDirection m_direction;
		

};


#endif //LINE_FOLLOWER_H