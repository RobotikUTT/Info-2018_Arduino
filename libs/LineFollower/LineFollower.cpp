

/** Includes **/
/**************/
#include "LineFollower.h"
/** Constructor **/
/*****************/

LineFollower::LineFollower(LineWatcher *watcher, MotorControl *motor, uint16_t max_photo_luminosity, Color *color)
{
	m_watcher = watcher;
	m_motor = motor;
	m_direction = FORWARD;
	m_max_photo_luminosity = max_photo_luminosity;
	m_crossroads_nb = 0;
	m_next_crossroads = UNKNOWN;
	m_color = color;
}

/** Public Methods **/
/********************/
void LineFollower::update(uint8_t &robotState)
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
	if (robotState == 0) //robot arrete avant depart (jusqu'a trigger)
	{
		m_motor->motorState(robotState);
	}
	else if (robotState == 2) //robot recherche ligne
	{
		if (m_watcher->photoState(0) == BLACK)	//m_watcher->photoBlackNb(BLACK) > 0
		{
			robotState++;
			m_motor->motorCrossroads(LEFT_CROSSROADS);
			delay(2000);
			m_motor->analogDirection(CENTER);
		}
		else
		{
			//m_motor->analogDirection(CENTER);
			m_motor->StartingCurve(*m_color);
		}


	}

	if(robotState == 3) //robot suit la ligne en faisant son parcourt
	{
		if ((m_watcher->lineSide() == LINE_CROSSROADS)/* && (m_direction == FORWARD)*/)
		{
			/*if (m_crossroads_nb == 0)
			{
				m_direction = FORWARDCROSSROADS;
				crossroadsEvent = 3;
				Serial.print("FORWARDCROSSROADS");
				m_motor->analogDirection(CENTER);
				delay(200);
			}
			else
			{
				m_direction = RIGHT;
				crossroadsEvent = 0;
				Serial.print("RIGHT");
			}*/
			
			//m_direction = LEFT;
			m_direction = m_next_crossroads;
			Serial.print("new crossroads");
		}
		else if ((m_watcher->photoBlackNb(BLACK) == 2) && (m_direction != FORWARD) && (m_direction != BACKWARD)) //on cherche si uniquement 2 photo sont noirs et espace de 1 photo : 10100 01010 00101
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
				m_next_crossroads = UNKNOWN;
				m_crossroads_nb++;
			}
		}
		/*if ((m_watcher->photoBlackNb(BLACK) > 2) && (m_direction == FORWARDCROSSROADS) && ((crossroadsEvent == 3) || (crossroadsEvent == 1)))
		{
			crossroadsEvent--;
			Serial.print("-------");
		}
		if ((m_watcher->photoBlackNb(BLACK) < 2) && (m_direction == FORWARDCROSSROADS) && (crossroadsEvent == 2))
		{
			crossroadsEvent--;
		}*/
		NextCrossroads(m_direction);
		Serial.print("cr event:");
		Serial.print(crossroadsEvent);
		Serial.print("  dir:");
		Serial.print(m_direction);
		Serial.print("  ");
	}
}

void LineFollower::NextCrossroads(CrossroadsDirection direction)
{
	uint16_t a = m_watcher->photoVal(2);
	if (m_max_photo_luminosity > a)
	{
		m_max_photo_luminosity = a;
	}

	switch (direction){
	case FORWARD:
		switch (m_watcher->lineSide()) {
		case LINE_LEFT:
			//m_motor->analogDirection(RIGHT_LINE, m_watcher->photoVal(0)-m_watcher->photoVal(4));//ligne a gauche : on tourne a droite
			m_motor->analogDirection(RIGHT_LINE, m_max_photo_luminosity-m_watcher->photoVal(2));//ligne a gauche : on tourne a droite
			Serial.print("line left");
			break;
		case LINE_RIGHT:
			//m_motor->analogDirection(LEFT_LINE, m_watcher->photoVal(4)-m_watcher->photoVal(0));
			m_motor->analogDirection(LEFT_LINE, m_max_photo_luminosity-m_watcher->photoVal(2));
			Serial.print("line left");
			break;
		default:
			//m_motor->direction(CENTER);//on va tout droit si on est au centre ou si on ne sait pas
			break;
		}
		break;
	case BACKWARD:  //A MODIFIER AU NIVEAU DES m_motor->analogDirection(...... (faire comme pour FORWARD)
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
	case FORWARDCROSSROADS:
		m_motor->analogDirection(CENTER);
		break;
	case UNKNOWN:
		m_motor->stop();
		break;
	}
}

uint8_t LineFollower::getCrossroadsNb()
{
	return m_crossroads_nb;
}

/** Private Methods **/
/*********************/

CrossroadsDirection LineFollower::getNextCrossroads()
{
	return m_next_crossroads;
}

void LineFollower::addNextCrossroads(CrossroadsDirection next_crossroads)
{
	m_next_crossroads = next_crossroads;
}