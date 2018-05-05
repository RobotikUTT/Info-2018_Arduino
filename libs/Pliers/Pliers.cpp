

/** Includes **/
/**************/
#include "Pliers.h"

/** Constructor **/
/*****************/
Pliers::Pliers(PololuA4983* stepper, int limit_switch_pin)
{
	m_stepper = stepper;
	m_limit_switch_pin = limit_switch_pin;

	pinMode(m_limit_switch_pin, INPUT);
	reset();
}

Pliers::~Pliers()
{
	delete m_stepper;
}

/** Public Methods **/
/********************/

void Pliers::update()
{
	if (digitalRead(m_limit_switch_pin) == LIMIT_SWITCH_PRESSED)
	{
		m_stepper->stop();
		m_current_level = MIN_LEVEL;
	}
	m_stepper->update();

	
	if ( m_currentAction.done )
	{
		if (m_actionList.count() > 0 )
		{
			m_currentAction = m_actionList.pop();

		}
	}
	else
	{
		if (m_currentAction.action == TAKE )
		{
			switch(m_currentAction.phase)
			{
				case PREPARING_ELEVATOR:
				{
					setLevel(m_currentAction.level+1);
					m_currentAction.phase = PREPARING_PLIERS;
					break;
				}
				case PREPARING_PLIERS:
				{
					if( m_stepper->fifoEmpty() )
					{
						open();
						m_currentAction.phase = ACTION_ELEVATOR;
					}
					break;
				}
				case ACTION_ELEVATOR:
				{
					setLevel(m_currentAction.level);
					m_currentAction.phase = ACTION_CUBE;
					break;
				}
				case ACTION_CUBE:
				{
					if ( m_stepper->fifoEmpty() )
					{
						close();
						m_currentAction.phase = RESET_ELEVATOR;
					}
					break;
				}
				case RESET_ELEVATOR:
				{
					setLevel(m_currentAction.level+1);
					m_currentAction.done = true;
					break;
				}
				default:
					break;
				
			}
		}
		else if ( m_currentAction.action == RELEASE )
		{
			switch(m_currentAction.phase)
			{
				case PREPARING_ELEVATOR:
				{
					setLevel(m_currentAction.level);
					m_currentAction.phase = ACTION_CUBE;
					break;
				}
				case ACTION_CUBE:
				{
					if (m_stepper->fifoEmpty())
					{
						open();
						m_currentAction.phase = RESET_ELEVATOR;
					}
					break;
				}
				case RESET_ELEVATOR:
				{
					setLevel(m_currentAction.level+1);
					m_currentAction.done = true;
					break;
				}
				default:
					break;
				
			}

		}
	}
	

}

void Pliers::reset()
{
	m_stepper->moveStep((MAX_LEVEL+1)*STEP_PER_LEVEL*DOWN);
	m_current_level = MIN_LEVEL;
}

void Pliers::setLevel(uint8_t level)
{
	int8_t dir;
	if (level < MIN_LEVEL)
	{
		level = MIN_LEVEL;
	}
	if (level > MAX_LEVEL)
	{
		level = MAX_LEVEL;
	}
	if (level > m_current_level)
	{
		dir = UP;
	}
	else
	{
		dir = DOWN;
	}
	m_stepper->moveStep(dir * level * STEP_PER_LEVEL);
	m_current_level = level;
}

void Pliers::open()
{
	ax12a.move(SERVO_ID, OPEN_ANGLE);
}

void Pliers::close()
{
	ax12a.move(SERVO_ID, CLOSED_ANGLE);
}

void Pliers::begin()
{
	NeoSWSerial rs485(AX12_RX,AX12_TX);
	AnySerial anySerial(&rs485);
	ax12a.begin(SERVO_BAUDRATE,SERVO_CONTROL_PIN,&anySerial);
	reset();
}

void Pliers::addAction(cube_action_t action, uint8_t level)
{
	pliers_action_t pliersAction;
	pliersAction.action = action;
	pliersAction.level = level;

	m_actionList.push(pliersAction);
}
/** Private Methods **/
/*********************/