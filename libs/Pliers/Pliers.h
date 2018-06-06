#ifndef PLIERS_H
#define PLIERS_H

/** Includes **/
/**************/

#include "Arduino.h"
#include "PololuA4983.h"
#include "Dynamixel_Serial.h"
#include "QueueList.h"
#include "canSender.h"
// #include "AX12.h"

/** Defines **/
/*************/

#define AX12_TX		2
#define AX12_RX		3

#define SERVO_ID 0x01               
#define SERVO_CONTROL_PIN A5
#define SERVO_BAUDRATE 57600 

#define OPEN_ANGLE		850
#define CLOSED_ANGLE	700

#define MIN_LEVEL 0
#define MAX_LEVEL 3
#define UP 	1
#define DOWN -1
#define STEP_PER_LEVEL 298
#define LIMIT_SWITCH_PRESSED	LOW
#define AX12_DELAY	300
#define CW_LIMIT_ANGLE 0x001 
#define CCW_LIMIT_ANGLE 0xFFF
#define AX12_SPEED		0x0FF //from 0x00 to 0x3FF
#define MAX_TORQUE		0x3FF	//same here


typedef enum{
	TAKE,
	RELEASE,
	SET_LEVEL
}cube_action_t;

typedef enum{
	PREPARING_ELEVATOR,
	PREPARING_PLIERS,
	ACTION_ELEVATOR,
	ACTION_CUBE,
	RESET_ELEVATOR,
	RESET_PLIERS
}action_phase_t;

typedef struct 
{
	cube_action_t action;
	uint8_t level;
	action_phase_t phase = PREPARING_ELEVATOR;
	bool done = false;
	
}pliers_action_t;

/** Class Descritpion **/

class Pliers
{
	private:
		PololuA4983* m_stepper;
		uint8_t m_limit_switch_pin;
		uint8_t m_current_level;
		pliers_action_t m_currentAction;
		QueueList<pliers_action_t> m_actionList;
		uint32_t m_ax12_millis;
		bool m_notify_fifo_empty;
		uint8_t m_lastSwitchState;


	public:
		Pliers(PololuA4983* stepper,int limit_switch_pin);
		~Pliers();
		void update();
		void setLevel(uint8_t level);
		void open();
		void close();
		void reset();
		void begin();
		void addAction(cube_action_t action, uint8_t level);


};


#endif //PLIERS_H