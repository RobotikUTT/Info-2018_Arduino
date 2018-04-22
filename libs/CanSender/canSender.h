/**
 * \file	sender.h
 * \author	Thomas Fuhrmann <tomesman@gmail.com>
 * \brief   Functions to send data accross the serial communication line
 * \date	06/12/2016
 * \copyright Copyright (c) 2016 UTCoupe All rights reserved.
 */

#ifndef CAN_SENDER_H
#define CAN_SENDER_H

#include <stdarg.h>
#include <stdint.h>
#include <QueueList.h>
#include "protocol.h"
#include "can.h"
#include "Arduino.h"
 #include "mcp2515.h"

#ifndef SENDER_ENUM
#define SENDER_ENUM
typedef enum
{
    SERIAL_ERROR = 0,
    SERIAL_INFO,
    SERIAL_DEBUG
} canSendEnum;
#endif

//#ifdef __cplusplus
//extern "C" void canSendC(canSendEnum level, String data);
//#endif
// typedef enum
// {
//     HANDSHAKE   =   0,
//     WHOAMI      =   1,
//     SET_MODE    =   3,
//     SPEED       =   4,
//     GET_CODER   =   5,
//     MANAGEMENT  =   7,
//     GOTOA       =   8,
//     GOTO        =   9,
//     ROT         =   10,
//     ROTNOMODULO =   11,
//     PIDLEFT     =   12,
//     PIDRIGHT    =   13,
//     PIDALL      =   14,
//     PWM         =   15,
//     SET_POS     =   16,
//     SET_PARAM   =   17,
//     CURRENT_POS =   18,
//     CURRENT_PWM =   19,
//     CURRENT_SPD =   20
// }canProtocolEnum;
extern MCP2515 mcp2515;
// extern can_frame canTxMsg;

class CanSender
{
public:
    CanSender();
    ~CanSender() {}
    //to be used everywhere
    //todo check why its only working with minimal 1 variadic argument
    static void canSend(uint8_t* message);
    // static void static void canSend(canProtocolEnum mode, String data);
    //to be used in the task
    static void canSendTask();
private:
    // static String CharArrayToString(const char * str, uint8_t size);
    static QueueList<uint8_t> dataToSend;
};

#endif //CAN_SENDER_H
