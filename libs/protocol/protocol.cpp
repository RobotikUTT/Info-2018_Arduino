//
// Created by Apache on 04/14/18.
//

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "protocol.h"
#include <stdlib.h>


bool flagArduinoConnected = false;

void encodeFrame(uint8_t* message, uint8_t mode, ...)
{

    for (uint8_t i = 0 ; i < MSG_SIZE ; i++)
    {
        message[i] = 0;
    }

    va_list argv;
    va_start(argv, mode);

    switch (mode) 
    {
        case CAN_HANDSHAKE:
        {           
            message[0] = mode;

            break;
        }

        case CAN_WHOAMI:
        {
            message[0] = mode;
            message[1] = ARDUINO_CAN_ADDR;
            break;
        }

        case CAN_SET_MODE:
        {
            message[0] = mode;
            break;
        }

        case CAN_SPD:
        {
            int linear, angular;
            unsigned int duration;
            linear = va_arg(argv, int);
            angular = va_arg(argv, int);
            duration = va_arg(argv, unsigned int);

            message[0] = CAN_SPD;
            message[1] = linear >> 8;
            message[2] = linear;
            message[3] = angular >> 8;
            message[4] = angular;
            message[5] = duration >> 8;
            message[6] = duration;
            
            break;
        }

        case CAN_GET_CODER:
        {

            int left_wheel_dist, right_wheel_dist;
            left_wheel_dist = va_arg(argv,int);
            right_wheel_dist = va_arg(argv,int);

            message[0] = CAN_GET_CODER;
            message[1] = left_wheel_dist >> 8;
            message[2] = left_wheel_dist;
            message[3] = right_wheel_dist >> 8;
            message[4] = right_wheel_dist;
            
            break;
        }

        case CAN_MANAGEMENT:
        {
          int order;
          order = va_arg(argv,int);
          switch (order)
          {
            case CAN_STOP:
            {
                message[0] = CAN_MANAGEMENT;
                message[1] = mode;
                
                break;
            }
    
            case CAN_START :
            {
                message[0] = CAN_MANAGEMENT;
                message[1] = CAN_START;
                
                break;
            }
    
            case CAN_PAUSE :
            {
                message[0] = CAN_MANAGEMENT;
                message[1] = CAN_PAUSE;
                
                break;
            }
    
            case CAN_RESUME :
            {
                message[0] = CAN_MANAGEMENT;
                message[1] = CAN_RESUME;
                
                break;
            }
    
            case CAN_RESET_ID :
            {
                message[0] = CAN_MANAGEMENT;
                message[1] = CAN_RESET_ID;
                
                break;
            }
    
            case CAN_SETEMERGENCYSTOP :
            {
                message[0] = CAN_MANAGEMENT;
                message[1] = CAN_SETEMERGENCYSTOP;
                
                break;
            }
    
            case CAN_NEXT_ORDER :
            {
                message[0] = CAN_MANAGEMENT;
                message[1] = CAN_NEXT_ORDER;
                
                break;
            }
    
            case CAN_RESET_ORDERS :
            {
                message[0] = CAN_MANAGEMENT;
                message[1] = CAN_RESET_ORDERS;
                
                break;
            }
            default:
              break;    
          }
            break;
        }

        

        case CAN_GOTOA :
        {
            int x, y, angle_int;
            unsigned char direction;
            x = va_arg(argv,int);
            y = va_arg(argv,int);
            angle_int = va_arg(argv, int);
            direction = va_arg(argv, int);

            message[0] = CAN_GOTOA;
            message[1] = x >> 8;
            message[2] = x;
            message[3] = y >> 8;
            message[4] = y;
            message[5] = angle_int >> 8;
            message[6] = angle_int;
            
            break;
        }

        case CAN_GOTO :
        {
            int x, y;
            x = va_arg(argv,int);
            y = va_arg(argv,int);
            
            message[0] = CAN_GOTO;
            message[1] = x >> 8;
            message[2] = x;
            message[3] = y >> 8;
            message[4] = y;
            break;
        }

        case CAN_ROT :
        {
            int angle_int;
            angle_int = va_arg(argv,int);

            message[0] = CAN_ROT;
            message[1] = angle_int >> 8;
            message[2] = angle_int;
            
            break;
        }

        case CAN_ROTNOMODULO :
        {
            int angle_int;
            angle_int = va_arg(argv,int);

            message[0] = CAN_ROTNOMODULO;
            message[1] = angle_int >> 8;
            message[2] = angle_int;
            break;
        }

        case CAN_PIDLEFT :
        case CAN_PIDRIGHT :
        case CAN_PIDALL :
        {
            unsigned int p,i,d;
            Serial.println("in case");
            p = va_arg(argv, unsigned int);
            Serial.println("p OK");
            i = va_arg(argv, unsigned int);
            Serial.println("I OK");
            d = va_arg(argv, unsigned int);
            Serial.println("D OK");
            
            message[0] = mode;
            message[1] = p >> 8;
            message[2] = p;
            message[3] = i >> 8;
            message[4] = i;
            message[5] = d >> 8;
            message[6] = d;
            
            break;
        }

        case CAN_PWM :
        case CAN_CURRENT_PWM :
        {
            int pwm_left, pwm_right;
            pwm_left = va_arg(argv,int);
            pwm_right = va_arg(argv,int);
            
            message[0] = mode;
            message[1] = pwm_left >> 8;
            message[2] = pwm_left;
            message[3] = pwm_right >> 8;
            message[4] = pwm_right;
            
            break;
        }

        case CAN_SET_POS :
        case CAN_CURRENT_POS :
        {
            int x, y, angle_int;
            x = va_arg(argv,int);
            y = va_arg(argv,int);
            angle_int = va_arg(argv,int);

            message[0] = mode;
            message[1] = x >> 8;
            message[2] = x;
            message[3] = y >> 8;
            message[4] = y;
            message[5] = angle_int >> 8;
            message[6] = angle_int;
            
            
            break;
        }

        case CAN_SET_PARAM :
        {
            unsigned int max_linear_speed, max_angular_speed,max_acceleration;
            max_linear_speed = va_arg(argv,unsigned int);
            max_angular_speed = va_arg(argv,unsigned int);
            max_acceleration = va_arg(argv,unsigned int);

            message[0] = CAN_SET_PARAM;
            message[1] = max_linear_speed >> 8;
            message[2] = max_linear_speed;
            message[3] = max_angular_speed >> 8;
            message[4] = max_angular_speed;
            message[5] = max_acceleration >> 8;
            message[6] = max_acceleration;
            
            break;
        }

        case CAN_CURRENT_SPD :
        {
            int linear_speed, left_wheel_speed, right_wheel_speed;
            linear_speed = va_arg(argv,int);
            left_wheel_speed = va_arg(argv,int);
            right_wheel_speed = va_arg(argv,int);
            message[0] = CAN_CURRENT_SPD;
            message[1] = linear_speed >> 8;
            message[2] = linear_speed;
            message[3] = left_wheel_speed >> 8;
            message[4] = left_wheel_speed;
            message[5] = right_wheel_speed >>8;
            message[6] = right_wheel_speed;
            
            break;
        }

        case CAN_MOVE_PLIERS :
        {
            unsigned char level;
            level = va_arg(argv, int);

            message[0] = CAN_MOVE_PLIERS;
            message[1] = level;
            
            break;
        }

        case CAN_CLOSE_OPEN_PLIERS :
        {
            unsigned char order;
            order = va_arg(argv, int);

            message[0] = CAN_CLOSE_OPEN_PLIERS;
            message[1] = order;
            
            break;
        }

        case CAN_SONAR_DISTANCE :
        {
            unsigned char sonar_id;
            unsigned int distance;
            sonar_id = va_arg(argv, int);
            distance = va_arg(argv,unsigned int);
            message[1] = sonar_id;
            message[2] = distance >> 8;
            message[3] = distance;
            
            break;
        }

        case CAN_THROW_BALLS :
        {
            message[0] = CAN_THROW_BALLS;
            
            break;
        }

        default:
            break;

    }

    va_end(argv);

}

void decodeFrame(uint8_t* message)
{
    uint8_t mode = message[0];
    switch (mode) 
    {
        case CAN_HANDSHAKE:
        {           
            Serial.println("HANDSHAKE");

            break;
        }

        case CAN_WHOAMI:
        {
            Serial.print("WHOAMI : ");
            switch(message[1])
            {
                case ALL_CAN_ADDR:
                    Serial.println("ALL");
                    break;

                case BBB_CAN_ADDR:
                    Serial.println("BBB");
                    break;

                case STM_CAN_ADDR:
                    Serial.println("STM");
                    break;

                case ARDUINO_CAN_ADDR:
                    Serial.println("ARD");
                    break;

                case ZIGBEE_CAN_ADDR:
                    Serial.println("ZIG B");
                    break;

                default:
                    break;
            }

            break;
        }

        case CAN_SET_MODE:
        {
            Serial.println("SET MODE");
            break;
        }

        case CAN_SPD :
        {
            int linear, angular;
            unsigned int duration;
            linear   = message[1] << 8 | message[2];
            angular  = message[3] << 8 | message[4];
            duration = message[5] << 8 | message[6];
            
            
            Serial.print("linear ");
            Serial.println(linear);
            Serial.print("angular ");
            Serial.println(angular);
            Serial.print("duration ");
            Serial.println(duration);
            
            break;
        }

        case CAN_GET_CODER :
        {

            int left_wheel_dist, right_wheel_dist;
            
            left_wheel_dist  = message[1] << 8 | message[2];
            right_wheel_dist = message[3] << 8 | message[4];

            Serial.print("l wheel dist ");
            Serial.println(left_wheel_dist);
            Serial.print("r wheel dist");
            Serial.println(left_wheel_dist);
            
            break;
        }

        case CAN_MANAGEMENT :
        {
            
          switch(message[1])
          {
            case CAN_STOP :
            {
    
                Serial.println("STOP");
                
                
                break;
            }
    
            case CAN_START :
            {
                Serial.println("START");
                
                
                break;
            }
    
            case CAN_PAUSE :
            {
                Serial.println("PAUSE");
                
                break;
            }
    
            case CAN_RESUME :
            {
                Serial.println("RESUME");
                
                break;
            }
    
            case CAN_RESET_ID :
            {
                Serial.println("RESET ID");
                
                break;
            }
    
            case CAN_SETEMERGENCYSTOP :
            {
                Serial.println("EMG STOP");
                
                break;
            }
    
            case CAN_NEXT_ORDER :
            {
                Serial.println(" NEXT ORDER");
                
                break;
            }
    
            case CAN_RESET_ORDERS :
            {
                Serial.println("RST ORDERS");
                
                break;
            }
            default:
              break;
          }
          
          break;
        }

        case CAN_GOTOA :
        {

            int x, y, angle_int;
            unsigned char direction;
            
            x           = message[1] << 8 | message[2];
            y           = message[3] << 8 | message[4];
            angle_int   = message[5] << 8 | message[6];
            direction   = message[7];
            

            Serial.println("GOTOA");
            Serial.print("x ");
            Serial.println(x);
            Serial.print("y ");
            Serial.println(y);
            Serial.print("angle");
            Serial.println((float)angle_int/FLOAT_PRECISION);

            
            break;
        }

        case CAN_GOTO :
        {
            int x, y;
            unsigned char direction;
            
            x           = message[1] << 8 | message[2];
            y           = message[3] << 8 | message[4];
            direction   = message[5];
            
            Serial.println("GOTO");
            Serial.print("x ");
            Serial.println(x);
            Serial.print("y ");
            Serial.println(y);
            Serial.print("direction");
            Serial.println(direction);
            break;
        }

        case CAN_ROT :
        {
            int angle_int;
            

            angle_int = message[1] << 8 | message[2];
            Serial.println("ROT");
            Serial.print("angle ");
            Serial.println((float)angle_int/FLOAT_PRECISION);

            break;
        }

        case CAN_ROTNOMODULO :
        {
            int angle_int;
            

            angle_int = message[1] << 8 | message[2];
            Serial.println("ROT NO MODULO");
            Serial.print("angle ");
            Serial.println((float)angle_int/FLOAT_PRECISION);
            break;
        }

        case CAN_PIDLEFT :
        case CAN_PIDRIGHT :
        case CAN_PIDALL :
        {
            unsigned int p,i,d;
            
            p = message[1] | message[2];
            i = message[3] | message[4];
            d = message[5] | message[6];
            if ( mode == CAN_PIDLEFT)
            {
                Serial.println("PID LEFT");
            }
            else if (mode == CAN_PIDRIGHT)
            {
                Serial.println("PID RIGHT");
            }
            else
            {
                Serial.println("PID ALL");
            }
            Serial.print("p ");
            Serial.println(p);
            Serial.print("i ");
            Serial.println(i);
            Serial.print("d ");
            Serial.println(d);
            
            
            break;
        }

        case CAN_PWM :
        case CAN_CURRENT_PWM :
        {
            int pwm_left, pwm_right;
            
            pwm_left  = message[1] << 8 | message[2];
            pwm_right = message[3] << 8 | message[4];

            if (mode == CAN_PWM)
            {
                Serial.print("PWM    ");
            }
            else
            {
                Serial.print("PWM|");
            }

            //Serial.print("left ");
            Serial.print(pwm_left);
            Serial.print("|");
            Serial.print(pwm_right);
            Serial.print("  ");
            
            break;
        }

        case CAN_SET_POS :
        case CAN_CURRENT_POS :
        {
            int x, y, angle_int;
            
            x           = message[1] << 8 | message[2];
            y           = message[3] << 8 | message[4];
            angle_int   = message[5] << 8 | message[6];
            
            if (mode == CAN_SET_POS)
            {
                Serial.print("SET POS    ");
            }
            else
            {
                Serial.print ("POS|");
            }

            //Serial.print("x ");
            Serial.print(x);
            Serial.print("|");
            //Serial.print("  y ");
            Serial.print(y);
            Serial.print("|");
            Serial.print(angle_int);
            Serial.print("  ");
            
            break;
        }

        case CAN_SET_PARAM :
        {
            unsigned int max_linear_speed, max_angular_speed,max_acceleration;
            
            max_linear_speed  = message[1] << 8 | message[2];
            max_angular_speed = message[3] << 8 | message[4];
            max_acceleration  = message[5] << 8 | message[6];

            Serial.println("SET PARAM");
            Serial.print("max lin spd ");
            Serial.println(max_linear_speed);
            Serial.print("max ang spd ");
            Serial.println(max_angular_speed);
            Serial.print("max acc");
            Serial.println(max_acceleration);
            
            break;
        }

        case CAN_CURRENT_SPD :
        {
            int linear_speed, left_wheel_speed, right_wheel_speed;
            linear_speed        = message[1] << 8 | message[2];
            left_wheel_speed    = message[3] << 8 | message[4];
            right_wheel_speed   = message[5] << 8 | message[5];
            
            Serial.print("SPD|");
            //Serial.print("lin   ");
            Serial.print(linear_speed);
            Serial.print("|");
            Serial.print(left_wheel_speed);
            Serial.print("|");
            Serial.println(right_wheel_speed);
            
            break;
        }

        case CAN_MOVE_PLIERS :
        {
            unsigned char level;
            level = message[1];

            Serial.println("MOVE PLIERS");
            Serial.print("lvl ");
            Serial.println(level);

        
            
            break;
        }

        case CAN_CLOSE_OPEN_PLIERS :
        {
            unsigned char order;
            order = message[1];
            if ( order > 0 )
            {
                Serial.println("CLOSE PLIERS");
            }
            else
            {
                Serial.println("OPEN PLIERS");
            }
            
            
            break;
        }

        case CAN_SONAR_DISTANCE :
        {
            unsigned char sonar_id;
            unsigned int distance;
            sonar_id = message[1];
            distance = message[2] << 8 | message[3];
            
            Serial.println("SONAR");
            Serial.print("id ");
            Serial.println(sonar_id);
            Serial.print("dist ");
            Serial.println(distance);
            
            break;
        }

        case CAN_THROW_BALLS :
        {
            
            Serial.println("THROW");
            
            break;
        }

        case CAN_OBJECT_ON_MAP:
        {
            Serial.print("OBJECT");
            uint8_t obj_ID;
            int16_t x,y;
            uint16_t radius;
            
            obj_ID = message[1];
            x      = message[2] << 8 | message[3];
            y      = message[4] << 8 | message[5];
            radius = message[6] << 8 | message[7];
            Serial.print("|");
            Serial.print(obj_ID);
            Serial.print("|");
            Serial.print(x);
            Serial.print("|");
            Serial.print(y);
            Serial.print("|");
            Serial.print(radius);
            Serial.println("|");
            break;
        }

        default:
            Serial.println("DATA");
            for (uint8_t i = 0; i< MSG_SIZE ; i++)
            {
              Serial.print("|");
              Serial.print(message[i]);
              
            }
            Serial.println("|");
            break;

    }
}
