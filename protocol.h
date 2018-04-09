//
// Created by tfuhrman on 09/05/17.
//

#ifndef ASSERV_PROTOCOL_H
#define ASSERV_PROTOCOL_H

// #include "parameters.h"
#include <stdint.h>



//  float are transmitted as integer
//  therefore any number refered as 
//  "decimal" is actually an int
//  multiplied by FLOAT_PRECISION

// BEGIN_ORDERS - Do not remove this comment

#define ALL_CAN_ADDR	0
#define BBB_CAN_ADDR	1
#define STM_CAN_ADDR	2
#define SELF_CAN_ADDR	3
#define ZIGBEE_CAN_ADDR	4


#define HANDSHAKE			0
#define WHOAMI				1
#define SET_MODE			2
#define MOVE_PLIERS 		21
#define CLOSE_OPEN_PLIERS	22
#define SONAR_DISTANCE		23
#define THROW_BALLS			24

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
// uint8_t getLog10(const uint16_t number);
// void ProtocolAutoSendStatus();

#endif //ASSERV_PROTOCOL_H
