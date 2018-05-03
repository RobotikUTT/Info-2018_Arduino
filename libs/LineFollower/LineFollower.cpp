

/** Includes **/
/**************/
#include "LineFollower.h"
/** Constructor **/
/*****************/

LineFollower::LineFollower(LineWatcher *watcher, MotorControl *motor)
{
	m_watcher = watcher;
	m_motor = motor;
	m_direction = FORDWARD;
}

/** Public Methods **/
/********************/
void LineFollower::update()
{
	//numerique :
	/*switch (m_watcher->lineCase()) {
	case LINE_LEFT:
		m_motor->direction(RIGHT_LINE);//ligne a gauche : on tourne a droite
		break;
	case LINE_RIGHT:
		m_motor->direction(LEFT_LINE);
		break;
	default:
		m_motor->direction(CENTER);//on va tout droit si on est au centre ou si on ne sait pas
		break;
	}*/

	NextCrossroads(m_direction);
}

void LineFollower::NextCrossroads(CrossroadsDirection direction)
{
	
	switch (direction){
	case FORDWARD:
		switch (m_watcher->lineSide()) {
		case LINE_LEFT:
			m_motor->analogDirection(RIGHT_LINE, m_watcher->photoVal(0)-m_watcher->photoVal(4));//ligne a gauche : on tourne a droite
			break;
		case LINE_RIGHT:
			m_motor->analogDirection(LEFT_LINE, m_watcher->photoVal(4)-m_watcher->photoVal(0));
			break;
		default:
			//m_motor->direction(CENTER);//on va tout droit si on est au centre ou si on ne sait pas
			break;
		}
		break;
	case BACKWARD:
		switch (m_watcher->lineSide()) {
		case LINE_RIGHT: //on inverse gauche et droite en marche arriere (et on prend l'inverse pour le calcul du dessous)
			m_motor->analogDirection(RIGHT_LINE, (m_watcher->photoVal(4)-m_watcher->photoVal(0))/2);//ligne a gauche : on tourne a droite
			break;
		case LINE_LEFT:
			m_motor->analogDirection(LEFT_LINE, (m_watcher->photoVal(0)-m_watcher->photoVal(4))/2);
			break;
		default:
			//m_motor->direction(CENTER);//on va tout droit si on est au centre ou si on ne sait pas
			break;
		}
		break;		
	}
}


/** Private Methods **/
/*********************/