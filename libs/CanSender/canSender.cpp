/**
 * \file	sender.c
 * \author	Thomas Fuhrmann <tomesman@gmail.com>
 * \brief   Functions to send data accross the serial communication line
 * \date	06/12/2016
 * \copyright Copyright (c) 2016 UTCoupe All rights reserved.
 */

#include "canSender.h"

/* Initialize static members */
QueueList<uint8_t> CanSender::dataToSend;

CanSender::CanSender() {
}


void CanSender::canSend(uint8_t* message)
{
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