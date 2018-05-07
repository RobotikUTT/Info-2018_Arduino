

/** Includes **/
/**************/
#include "LineFollower.h"
/** Constructor **/
/*****************/

LineFollower::LineFollower(LineWatcher *watcher, MotorControl *motor)
{
	m_watcher = watcher;
	m_motor = motor;
	m_direction = FORWARD;
}

/** Public Methods **/
/********************/
void LineFollower::update(uint8_t robotState)
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
	if (robotState == 0)
	{
		m_motor->motorState(robotState);
	}
	else
	{
		if (m_watcher->lineSide() == LINE_CROSSROADS)
		{
			m_direction = LEFT;
		}
		else if (m_watcher->photoBlackNb() == 2) //on cherche si uniquement 2 photo sont noirs et espace de 1 photo : 10100 01010 00101
		{										//cela sert a soir si on est sur la ligne apres le croisement (si on a finit de tourner)
			uint8_t local_var = 0;
			for (int i = 0; i < 2; ++i)
			{
				if ((m_watcher->photoState(i) == BLACK) && (m_watcher->photoState(i+2) == BLACK))
				{
					local_var = 1;
				}
			}
			if (local_var == 1)
			{
				m_direction = FORWARD;
			}
		}
		NextCrossroads(m_direction);
		Serial.print("**");
		Serial.print(m_direction);
	}
}

void LineFollower::NextCrossroads(CrossroadsDirection direction)
{
	
	switch (direction){
	case FORWARD:
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
	case LEFT:
		m_motor->motorCrossroads(LEFT_CROSSROADS);
		break;
	case RIGHT:
		m_motor->motorCrossroads(RIGHT_CROSSROADS);
		break;	
	}
}


/** Private Methods **/
/*********************/