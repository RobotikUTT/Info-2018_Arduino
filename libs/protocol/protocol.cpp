//
// Created by Apache on 04/14/18.
//

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "protocol.h"
#include <stdlib.h>


bool flagArduinoConnected = false;

uint8_t encodeFrame(uint8_t* message, uint8_t mode, ...)
{
    uint8_t message_size = 0;

    for (uint8_t i = 0 ; i < MSG_SIZE ; i++)
    {
        message[i] = 0;
    }

    va_list argv;
    va_start(argv, mode);

    switch (mode) 
    {
        case HANDSHAKE:
        {           
            message[0] = HANDSHAKE;
            message_size = 1;

            break;
        }

        case WHOAMI:
        {
            message[0] = WHOAMI;
            message[1] = ARDUINO_CAN_ADDR;
            message_size = 2;
            break;
        }

        case SET_MODE:
        {
            message[0] = SET_MODE;
            message_size = 1;
            break;
        }

        case SPD:
        {
            int linear, angular;
            unsigned int duration;
            linear = va_arg(argv, int);
            angular = va_arg(argv, int);
            duration = va_arg(argv, unsigned int);

            message[0] = SPD;
            message[1] = linear >> 8;
            message[2] = linear;
            message[3] = angular >> 8;
            message[4] = angular;
            message[5] = duration >> 8;
            message[6] = duration;
            message_size = 7;
            
            break;
        }

        case GET_CODER:
        {

            int left_wheel_dist, right_wheel_dist;
            left_wheel_dist = va_arg(argv,int);
            right_wheel_dist = va_arg(argv,int);

            message[0] = GET_CODER;
            message[1] = left_wheel_dist >> 8;
            message[2] = left_wheel_dist;
            message[3] = right_wheel_dist >> 8;
            message[4] = right_wheel_dist;
            
            message_size = 5;
            break;
        }

        case MANAGEMENT:
        {
          int order;
          order = va_arg(argv,int);
          switch (order)
          {
            message_size = 2;
            case STOP:
            {
                message[0] = MANAGEMENT;
                message[1] = STOP;
                
                break;
            }
    
            case START :
            {
                message[0] = MANAGEMENT;
                message[1] = START;
                
                break;
            }
    
            case PAUSE :
            {
                message[0] = MANAGEMENT;
                message[1] = PAUSE;
                
                break;
            }
    
            case RESUME :
            {
                message[0] = MANAGEMENT;
                message[1] = RESUME;
                
                break;
            }
    
            case RESET_ID :
            {
                message[0] = MANAGEMENT;
                message[1] = RESET_ID;
                
                break;
            }
    
            case SETEMERGENCYSTOP :
            {
                message[0] = MANAGEMENT;
                message[1] = SETEMERGENCYSTOP;
                
                break;
            }
    
            case NEXT_ORDER :
            {
                message[0] = MANAGEMENT;
                message[1] = NEXT_ORDER;
                
                break;
            }
    
            case RESET_ORDERS :
            {
                message[0] = MANAGEMENT;
                message[1] = RESET_ORDERS;
                
                break;
            }

            case UNSETEMERGENCYSTOP :
            {
                message[0] = MANAGEMENT;
                message[1] = UNSETEMERGENCYSTOP;
                
                break;
            }

            default:
            {
                message_size = 0;
                break;    
            }
          }

            break;
        }

        

        case GOTOA :
        {
            int x, y, angle_int;
            unsigned char direction;
            x = va_arg(argv,int);
            y = va_arg(argv,int);
            angle_int = va_arg(argv, int);
            direction = va_arg(argv, int);

            message[0] = GOTOA;
            message[1] = x >> 8;
            message[2] = x;
            message[3] = y >> 8;
            message[4] = y;
            message[5] = angle_int >> 8;
            message[6] = angle_int;
            
            message_size = 7;

            break;
        }

        case GOTO :
        {
            int x, y;
            x = va_arg(argv,int);
            y = va_arg(argv,int);
            
            message[0] = GOTO;
            message[1] = x >> 8;
            message[2] = x;
            message[3] = y >> 8;
            message[4] = y;

            message_size = 5;
            break;
        }

        case ROT :
        {
            int angle_int;
            angle_int = va_arg(argv,int);

            message[0] = ROT;
            message[1] = angle_int >> 8;
            message[2] = angle_int;
            
            message_size = 3;

            break;
        }

        case ROTNOMODULO :
        {
            int angle_int;
            angle_int = va_arg(argv,int);

            message[0] = ROTNOMODULO;
            message[1] = angle_int >> 8;
            message[2] = angle_int;

            message_size = 3;

            break;
        }

        case PIDLEFT :
        case PIDRIGHT :
        case PIDALL :
        {
            unsigned int p,i,d;
            p = va_arg(argv, unsigned int);
            i = va_arg(argv, unsigned int);
            d = va_arg(argv, unsigned int);
            
            message[0] = mode;
            message[1] = p >> 8;
            message[2] = p;
            message[3] = i >> 8;
            message[4] = i;
            message[5] = d >> 8;
            message[6] = d;

            message_size = 7;
            
            break;
        }

        case PWM :
        case CURRENT_PWM :
        {
            int pwm_left, pwm_right;
            pwm_left = va_arg(argv,int);
            pwm_right = va_arg(argv,int);
            
            message[0] = mode;
            message[1] = pwm_left >> 8;
            message[2] = pwm_left;
            message[3] = pwm_right >> 8;
            message[4] = pwm_right;

            message_size = 5;
            
            break;
        }

        case SET_POS :
        case CURRENT_POS :
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
            
            message_size = 7;
            
            break;
        }

        case SET_PARAM :
        {
            unsigned int max_linear_speed, max_angular_speed,max_acceleration;
            max_linear_speed = va_arg(argv,unsigned int);
            max_angular_speed = va_arg(argv,unsigned int);
            max_acceleration = va_arg(argv,unsigned int);

            message[0] = SET_PARAM;
            message[1] = max_linear_speed >> 8;
            message[2] = max_linear_speed;
            message[3] = max_angular_speed >> 8;
            message[4] = max_angular_speed;
            message[5] = max_acceleration >> 8;
            message[6] = max_acceleration;

            message_size = 7;
            
            break;
        }

        case CURRENT_SPD :
        {
            int linear_speed, left_wheel_speed, right_wheel_speed;
            linear_speed = va_arg(argv,int);
            left_wheel_speed = va_arg(argv,int);
            right_wheel_speed = va_arg(argv,int);
            message[0] = CURRENT_SPD;
            message[1] = linear_speed >> 8;
            message[2] = linear_speed;
            message[3] = left_wheel_speed >> 8;
            message[4] = left_wheel_speed;
            message[5] = right_wheel_speed >>8;
            message[6] = right_wheel_speed;
            
            message_size = 7;

            break;
        }
        case MOVE_PLIERS :
        {
            unsigned char level;
            level = va_arg(argv, int);

            message[0] = MOVE_PLIERS;
            message[1] = level;
            
            message_size = 2;

            break;
        }

        case CLOSE_OPEN_PLIERS :
        {
            unsigned char order;
            order = va_arg(argv, int);

            message[0] = CLOSE_OPEN_PLIERS;
            message[1] = order;
            
            message_size = 2;

            break;
        }
        case SONAR_DISTANCE :
        {
            uint8_t distance_sonar_0;
            uint8_t distance_sonar_1;
            uint8_t distance_sonar_2;
            uint8_t distance_sonar_3;
            uint8_t distance_sonar_4;
            
            distance_sonar_0 = va_arg(argv, int);
            distance_sonar_1 = va_arg(argv, int);
            distance_sonar_2 = va_arg(argv, int);
            distance_sonar_3 = va_arg(argv, int);
            distance_sonar_4 = va_arg(argv, int);

            message[0] = SONAR_DISTANCE;
            message[1] = distance_sonar_0;
            message[2] = distance_sonar_1;
            message[3] = distance_sonar_2;
            message[4] = distance_sonar_3;
            message[5] = distance_sonar_4;
            
            message_size = 6;

            break;
        }
        case THROW_BALLS :
        {
            message[0] = THROW_BALLS;
                
            message_size = 1;

            break;
        }

        case ORDER_COMPLETED:
        {
            message[0] = ORDER_COMPLETED;
            message[1] = 2;
            message_size = 2;
            break;
        }

        case SET_SERVO:
        {
            uint8_t sonarID;
            uint8_t angle;

            sonarID = va_arg(argv, int);
            angle = va_arg(argv, int);

            message[0] = SET_SERVO;
            message[1] = sonarID;
            message[2] = angle;

            message_size = 3;
            break;
        }

        default:
            break;

    }

    va_end(argv);

    return message_size;
}
