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



#define CAN_STOP 				0
#define CAN_START 				1
#define CAN_PAUSE 				2
#define CAN_RESUME 				3
#define CAN_RESET_ID 			4
#define CAN_SETEMERGENCYSTOP 	5
#define CAN_NEXT_ORDER			6
#define CAN_RESET_ORDERS		7


#define CAN_HANDSHAKE			0
#define CAN_WHOAMI				1
#define CAN_SET_MODE			2

#define CAN_SPD 				4
#define CAN_GET_CODER			5
#define CAN_MANAGEMENT			7
#define CAN_GOTOA				8
#define CAN_GOTO 				9
#define CAN_ROT 				10
#define CAN_ROTNOMODULO 		11
#define CAN_PIDLEFT				12
#define CAN_PIDRIGHT			13
#define CAN_PIDALL				14
#define CAN_PWM 				15
#define CAN_SET_POS				16
#define CAN_SET_PARAM			17
#define CAN_CURRENT_POS 		18
#define CAN_CURRENT_PWM			19
#define CAN_CURRENT_SPD			20
#define CAN_MOVE_PLIERS 		21
#define CAN_CLOSE_OPEN_PLIERS	22
#define CAN_SONAR_DISTANCE		23
#define CAN_THROW_BALLS			24
#define CAN_OBJECT_ON_MAP		25

#define HANDSHAKE   'h'
#define START       'S'     //no args, start the program
#define HALT        'H'     //no args, halt the program
#define	GOTOA 		'c' 	// x(int);y(int);a(decimal);direction(int) - (mm and radian), direction is optionnal : 1 is forward, -1 is backward, 0 is any
#define	GOTO 		'd' 	// x(int);y(int);direction(int) - (mm), direction is optionnal : 1 is forward, -1 is backward, 0 is any
#define	ROT 		'e' 	// a(decimal) - (radian), can't turn more than 1 turn
#define ROTNOMODULO	'a' 	// a(decimal) - radian, can turn more than 1 turn
#define	KILLG 		'f' 	// no args, go to next order
#define	CLEANG 		'g' 	// no args, cancel all orders
#define	PIDLEFT		'p' 	// p(decimal);i(decimal);d(decimal) - set left PID
#define	PIDRIGHT	'i' 	// p(decimal);i(decimal);d(decimal) - set right PID
#define PIDALL 		'u' 	// p(decimal);i(decimal);d(decimal) - set both PID
#define	GET_CODER 	'j' 	// no args, response : l(long);r(long)
#define	PWM 		'k' 	// l(int);r(int);duration(int) - set left and right pwm for duration ms
#define	SPD 		'b' 	// l(int);a(int);duration(int) - set linear and angular spd for duration ms
#define	ACCMAX 		'l' 	// a(int) - set max acceleration (mm/s-2)
#define	SPDMAX 		'x' 	// v(int),r(decimal) - set max spd (mm/s) and rotation ratio
#define	SET_POS		'm' 	// x(int);y(int);a(decimal) - set pos (mm / radians)
#define	GET_POS		'n' 	// no args, response : x(int);y(int);a(decimal) - get current pos (mm and radians)
#define GET_SPD 	'y' 	// no args, respond : l(int);r(int) - get wheels speed (mm/s)
#define GET_TARGET_SPD 	'v'	// no args, respond : l(int);r(int) - get target wheels speed (mm/s)
#define	GET_POS_ID 	'o'		// no args, response : x(int);y(int);a(decimal);id(int) - get current pos and last id (mm and radians)
#define GET_LAST_ID	't' 	// no args, response : id(int)
#define	PAUSE 		'q' 	// no args, pauses control
#define	RESUME 		'r'		// no args, resumes control
#define RESET_ID 	's' 	// no args, reset last finished id to 0
#define PINGPING 	'z'		// no args, switch led state
#define WHOAMI 		'w' 	// no args, answers 'ASSERV' or 'PAP'
#define SETEMERGENCYSTOP 'A'	// enable(int)

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
void encodeFrame(uint8_t* message,uint8_t mode, ...);
void decodeFrame(uint8_t* message);

// uint8_t getLog10(const uint16_t number);
// void ProtocolAutoSendStatus();

#endif //ASSERV_PROTOCOL_H
