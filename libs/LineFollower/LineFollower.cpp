

/** Includes **/
/**************/
#include "LineFollower.h"
/** Constructor **/
/*****************/

LineFollower::LineFollower(LineWatcher *watcher, MotorControl *motor)
{
	m_watcher = watcher;
	m_motor = motor;
}

/** Public Methods **/
/********************/
void LineFollower::update()
{
	switch (m_watcher->lineCase()) {
	case LINE_CENTER:
		
		break;
	case LINE_LEFT:
		
		break;
	case LINE_RIGHT:

		break;
	default:
		break;
	}
}


/** Private Methods **/
/*********************/