

/** Includes **/
/**************/
#include "Pliers.h"

/** Constructor **/
/*****************/
Pliers::Pliers(PololuA4983* stepper, int limit_switch_pin)
{
	m_stepper = stepper;
	m_limit_switch_pin = limit_switch_pin;
	m_current_level = MIN_LEVEL;
	pinMode(m_limit_switch_pin, INPUT);
	m_currentAction.done = true;
	m_notify_fifo_empty = false;
	m_lastSwitchState = digitalRead(m_limit_switch_pin);
}

Pliers::~Pliers()
{
	delete m_stepper;
}

/** Public Methods **/
/********************/

void Pliers::update()
{	
	uint8_t _switchValue = digitalRead(m_limit_switch_pin);
	if (_switchValue != m_lastSwitchState )
	{
		if ( _switchValue == LIMIT_SWITCH_PRESSED )
		{
			Serial.print("rising edge");
			m_current_level = MIN_LEVEL;
			m_stepper->stop();
			
		}
		m_lastSwitchState = _switchValue;
	}
	m_stepper->update();

	
	if ( m_currentAction.done )
	{

		if (m_actionList.count() > 0 )
		{
			m_currentAction = m_actionList.pop();
			m_notify_fifo_empty = true;
		}
		else
		{
			if (m_notify_fifo_empty)
			{
				uint8_t answer[MSG_SIZE];
	            encodeFrame(answer, ORDER_COMPLETED);
	        	CanSender::canSend(answer);
	        	m_notify_fifo_empty = false;
			}
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
					//Serial.print("level:");
					//Serial.println(m_currentAction.level);
					m_currentAction.phase = PREPARING_PLIERS;
					//Serial.println("done prepare elevator");
					break;
				}
				case PREPARING_PLIERS:
				{
					//Serial.println("prepare pliers");
					if( m_stepper->fifoEmpty() )
					{
						// Serial.println("fifo empty here also");
						open();
						m_ax12_millis = millis();
						m_currentAction.phase = ACTION_ELEVATOR;
						//Serial.println("done prepare pliers");
					}
					break;
				}
				case ACTION_ELEVATOR:
				{
					if (millis() - m_ax12_millis > AX12_DELAY)
					{
						setLevel(m_currentAction.level);
						m_currentAction.phase = ACTION_CUBE;
						//Serial.println("done action elevator");
					}
					break;
				}
				case ACTION_CUBE:
				{
					if ( m_stepper->fifoEmpty() )
					{
						close();
						m_ax12_millis = millis();
						m_currentAction.phase = RESET_ELEVATOR;
						//Serial.println("done action cube");
					}
					break;
				}
				case RESET_ELEVATOR:
				{
					if (millis() - m_ax12_millis > AX12_DELAY)
					{
						setLevel(m_currentAction.level+1);
						m_currentAction.done = true;
						//Serial.println("done reset elevator");
					}
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
				// case RESET_ELEVATOR:
				// {
				// 	setLevel(m_currentAction.level+1);
				// 	m_currentAction.done = true;
				// 	break;
				// }
				default:
					break;
				
			}

		}
		else if ( m_currentAction.action == SET_LEVEL)
		{
			setLevel(m_currentAction.level);
			m_currentAction.done = true;
		}
	}
	

}

void Pliers::reset()
{
	close();
	//m_current_level = MAX_LEVEL;
	//setLevel(MIN_LEVEL);
	m_stepper->moveStep(50);
	m_stepper->moveStep(-200);
}

void Pliers::setLevel(uint8_t level)
{
	int8_t dir;
	Serial.print("set level ");
	Serial.println(level);
	Serial.print("current level: ");
	Serial.println(m_current_level);
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
	int16_t nb_steps;
	nb_steps = (level - m_current_level) * STEP_PER_LEVEL;
	m_stepper->moveStep(nb_steps);
	Serial.print("nb_steps: ");
	Serial.println(nb_steps);
	m_current_level = level;
}

void Pliers::open()
{
	// Serial.println("before open");
	//delay(10);
	//ax12a.move(SERVO_ID, OPEN_ANGLE);
	//Serial.println("open");
	// Dynamixel.setHoldingTorque(SERVO_ID, false);
	Dynamixel.servo(SERVO_ID,OPEN_ANGLE,AX12_SPEED);
}

void Pliers::close()
{
	//Dynamixel.setHoldingTorque(SERVO_ID, true);
	Dynamixel.servo(SERVO_ID, CLOSED_ANGLE,AX12_SPEED);
	//Serial.println("close");
}

void Pliers::begin()
{
	//NeoSWSerial rs485(AX12_RX,AX12_TX);
	// ax12a.begin(SERVO_BAUDRATE,SERVO_CONTROL_PIN,&Serial3);
	// ax12a.setMaxTorque(SERVO_ID, 0x3FF);


	Dynamixel.begin(Serial3, SERVO_BAUDRATE);                                    
    Dynamixel.setDirectionPin(SERVO_CONTROL_PIN);                            
    Dynamixel.setMode(SERVO_ID, SERVO, CW_LIMIT_ANGLE, CCW_LIMIT_ANGLE);    
    Dynamixel.setMaxTorque(SERVO_ID, MAX_TORQUE);
    Dynamixel.setHoldingTorque(SERVO_ID, true);
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