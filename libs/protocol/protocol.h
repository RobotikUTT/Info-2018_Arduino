//
// Created by tfuhrman on 09/05/17.
//

#ifndef ASSERV_PROTOCOL_H
#define ASSERV_PROTOCOL_H

// #include "parameters.h"
#include <stdint.h>
#include "Arduino.h"



//  float are transmitted as integer
//  therefore any number refered as 
//  "decimal" is actually an int
//  multiplied by FLOAT_PRECISION

// BEGIN_ORDERS - Do not remove this comment

#define ALL_CAN_ADDR		0
#define BBB_CAN_ADDR		1
#define STM_CAN_ADDR		2
#define ARDUINO_CAN_ADDR	3
#define ZIGBEE_CAN_ADDR		4



#define STOP 				0
#define START 				1
#define PAUSE 				2
#define RESUME 				3
#define RESET_ID 			4
#define SETEMERGENCYSTOP 	5
#define NEXT_ORDER			6
#define RESET_ORDERS		7
#define UNSETEMERGENCYSTOP	8


#define HANDSHAKE			0
#define WHOAMI				1
#define SET_MODE			2

#define SPD 				4
#define GET_CODER			5
#define MANAGEMENT			7
#define GOTOA				8
#define GOTO 				9
#define ROT 				10
#define ROTNOMODULO 		11
#define PIDLEFT				12
#define PIDRIGHT			13
#define PIDALL				14
#define PWM 				15
#define SET_POS				16
#define SET_PARAM			17
#define CURRENT_POS 		18
#define CURRENT_PWM			19
#define CURRENT_SPD			20
#define MOVE_PLIERS 		21
#define CLOSE_OPEN_PLIERS	22
#define SONAR_DISTANCE		23
#define THROW_BALLS			24
#define OBJECT_ON_MAP		25
#define ORDER_COMPLETED		26
#define SET_SERVO			27
#define ROBOT_BLOCKED		28
#define ACTION_PLIERS		29

#define	PLIERS_TAKE			1
#define PLIERS_RELEASE		0


#define SERIAL_HANDSHAKE   'h'
#define SERIAL_START       'S'     //no args, start the program
#define SERIAL_HALT        'H'     //no args, halt the program
#define	SERIAL_GOTOA 		'c' 	// x(int);y(int);a(decimal);direction(int) - (mm and radian), direction is optionnal : 1 is forward, -1 is backward, 0 is any
#define	SERIAL_GOTO 		'd' 	// x(int);y(int);direction(int) - (mm), direction is optionnal : 1 is forward, -1 is backward, 0 is any
#define	SERIAL_ROT 		'e' 	// a(decimal) - (radian), can't turn more than 1 turn
#define SERIAL_ROTNOMODULO	'a' 	// a(decimal) - radian, can turn more than 1 turn
#define	SERIAL_KILLG 		'f' 	// no args, go to next order
#define	SERIAL_CLEANG 		'g' 	// no args, cancel all orders
#define	SERIAL_PIDLEFT		'p' 	// p(decimal);i(decimal);d(decimal) - set left PID
#define	SERIAL_PIDRIGHT	'i' 	// p(decimal);i(decimal);d(decimal) - set right PID
#define SERIAL_PIDALL 		'u' 	// p(decimal);i(decimal);d(decimal) - set both PID
#define	SERIAL_GET_CODER 	'j' 	// no args, response : l(long);r(long)
#define	SERIAL_PWM 		'k' 	// l(int);r(int);duration(int) - set left and right pwm for duration ms
#define	SERIAL_SPD 		'b' 	// l(int);a(int);duration(int) - set linear and angular spd for duration ms
#define	SERIAL_ACCMAX 		'l' 	// a(int) - set max acceleration (mm/s-2)
#define	SERIAL_SPDMAX 		'x' 	// v(int),r(decimal) - set max spd (mm/s) and rotation ratio
#define	SERIAL_SET_POS		'm' 	// x(int);y(int);a(decimal) - set pos (mm / radians)
#define	SERIAL_GET_POS		'n' 	// no args, response : x(int);y(int);a(decimal) - get current pos (mm and radians)
#define SERIAL_GET_SPD 	'y' 	// no args, respond : l(int);r(int) - get wheels speed (mm/s)
#define SERIAL_GET_TARGET_SPD 	'v'	// no args, respond : l(int);r(int) - get target wheels speed (mm/s)
#define	SERIAL_GET_POS_ID 	'o'		// no args, response : x(int);y(int);a(decimal);id(int) - get current pos and last id (mm and radians)
#define SERIAL_GET_LAST_ID	't' 	// no args, response : id(int)
#define	SERIAL_PAUSE 		'q' 	// no args, pauses control
#define	SERIAL_RESUME 		'r'		// no args, resumes control
#define SERIAL_RESET_ID 	's' 	// no args, reset last finished id to 0
#define SERIAL_PINGPING 	'z'		// no args, switch led state
#define SERIAL_WHOAMI 		'w' 	// no args, answers 'ASSERV' or 'PAP'
#define SERIAL_SETEMERGENCYSTOP 'A'	// enable(int)
#define SERIAL_UNSETEMERGENCYSTOP 'B'

// END_ORDERS - Do not remove this comment

#define AUTO_SEND	'~'		// x(int);y(int);a(decimal)
#define JACK 	'J'

#define JACK_SEND_NR 5
#define FLOAT_PRECISION 1000.0
#define FAILED_MSG "FAILED"
#define MAX_COMMAND_LEN 60
#define MAX_ID_VAL 32767
#define MAX_ID_LEN 5
#define ID_START_INDEX 2
#define MAX_RESPONSE_LEN 50

#define MSG_SIZE	8
#define BAUDRATE	57600



// #define MAX_BYTES_PER_IT (0.9*BAUDRATE/(HZ*10))

#define ORDER_INDEX (uint8_t)0
#define ID_INDEX    (uint8_t)2

#ifdef DEBUG_TARGET_SPEED
#define MAX_AUTOSEND_SIZE (48)
#else
#define MAX_AUTOSEND_SIZE (24)
#endif

extern bool flagArduinoConnected;
void parseAndExecuteOrder(uint8_t* message);
uint8_t encodeFrame(uint8_t* message,uint8_t mode, ...);

// uint8_t getLog10(const uint16_t number);
// void ProtocolAutoSendStatus();

#endif //ASSERV_PROTOCOL_H
