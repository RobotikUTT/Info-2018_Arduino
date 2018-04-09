//
// Created by tfuhrman on 09/05/17.
//

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "protocol.h"
#include <stdlib.h>
#include "canSender.h"

//}

void autoSendStatus() {
    //todo auto_send ?
    // CanSender::canSend(CURRENT_POS, 
    //     (int)current_pos.x, 
    //     (int)current_pos.y,
    //     (int)(current_pos.angle*FLOAT_PRECISION));
    
    

#if DEBUG_TARGET_SPEED
//    index += sprintf(message+index, ";%i;%i;%i;%i",
//			(int)wheels_spd.left,
//			(int)wheels_spd.right,
//			(int)(control.speeds.linear_speed - control.speeds.angular_speed),
//			(int)(control.speeds.linear_speed + control.speeds.angular_speed));
#endif
}


void ProtocolAutoSendStatus() {
    autoSendStatus();
// #if AUTO_STATUS_HZ
//     static int i=0;
//     if (++i % (HZ / AUTO_STATUS_HZ) == 0) {
//         autoSendStatus();
//     }
// #endif
}



bool flagArduinoConnected = false;

//order is order;id_servo;params
void parseAndExecuteOrder(uint8_t* message) {
    uint8_t mode = message[0];

    switch (mode) 
    {
        case HANDSHAKE:
        {
            // Ack that stm has started
            Serial.print("HANDSHAKE");
            Serial.print("\n");
            CanSender::canSend(WHOAMI,SELF_CAN_ADDR);
            flagArduinoConnected = true;
            //g_serial.print("HANDSHAKE\n");
            // CanSender::canSend(SERIAL_INFO, "%d;", order_id);
            // CanSender::canSend(SERIAL_DEBUG, "Arduino %s has started (%d)", ARDUINO_ID, order_id);
            
            break;
        }

        // case WHOAMI:
        // {
        //     break;
        // }

        case SET_MODE:
        {
            break;
        }

        case MOVE_PLIERS:
        {
            Serial.print("MOVE PLIERS");
            
            uint8_t level = message[1];
            
            break;
        }

        case CLOSE_OPEN_PLIERS:
        {
            // CanSender::canSend(GET_CODER,get_left_encoder(), get_right_encoder());
            break;
        }

    

        case SONAR_DISTANCE:
        {
            Serial.print("SONAR DISTANCE");
            
            break;
        }

        case THROW_BALLS:
        {
            Serial.print("THROW BALLS");
            
            break;
        }
        default:
            break;
        

    }
}
