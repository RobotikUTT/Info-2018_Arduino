/**
 * \file	sender.c
 * \author	Thomas Fuhrmann <tomesman@gmail.com>
 * \brief   Functions to send data accross the serial communication line
 * \date	06/12/2016
 * \copyright Copyright (c) 2016 UTCoupe All rights reserved.
 */

#include "canSender.h"


//#include "QueueList.h"

/* Initialize static members */
QueueList<uint8_t> CanSender::dataToSend;

CanSender::CanSender() {
}

// void CanSender::canSend(canProtocolEnum mode, String data) {
//     if (mode <= DEBUG_LEVEL & data != "") {
//         dataToSend.push(data);
//     }
// }

void CanSender::canSend(uint8_t mode, ...)
{
    uint8_t i, j, count = 0;
    uint8_t message[] = {0, 0, 0, 0, 0, 0, 0, 0};
    // String serialData, tmpString = "";
        va_list argv;
        va_start(argv, mode);
        switch(mode)        
        {
            case WHOAMI:
                int id;
                id = va_arg(argv, int);
                message[0] = mode;
                message[1] = id;
                break;

            case SONAR_DISTANCE:
                int sonar_id ;
                unsigned int distance;
                

                sonar_id = va_arg(argv, int);
                distance = va_arg(argv, unsigned int);
                message[0] = mode;
                // TODO: find how to cast in the right type
                //MSB first
                message[1] = sonar_id;
                
                message[2] = distance >> 8;
                message[3] = distance;

                break;

            case THROW_BALLS:
                
                message[0] = mode;

                break;

            default:
                break;

        }

    va_end(argv);

    for (uint8_t i = 0 ; i < 8 ; i++)
    {
        dataToSend.push(message[i]);    
    }
}

void CanSender::canSendTask() {
    can_frame canTxMsg;
    canTxMsg.can_id = BBB_CAN_ADDR;
    canTxMsg.can_dlc = 8;
    while (!dataToSend.isEmpty()) {
    
        for (int8_t i = 0; i < 8 ; i++)
        {
            canTxMsg.data[i] = dataToSend.pop();
        }

        MCP2515::ERROR txError = mcp2515.sendMessage(&canTxMsg);
        
    }
}