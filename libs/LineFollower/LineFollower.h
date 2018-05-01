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

/** Class Descritpion **/

class LineFollower
{
	private:
		LineWatcher* m_watcher;
		MotorControl* m_motor;

	public:
		LineFollower(LineWatcher *watcher, MotorControl *motor);
		void update();
		

};


#endif //LINE_FOLLOWER_H