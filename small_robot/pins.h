#ifndef PINS_H
#define PINS_H
#include "Arduino.h"

#define PHOTO_0		A0
#define PHOTO_1		A1
#define PHOTO_2		A6
#define PHOTO_3		A2
#define PHOTO_4		A3

#define LEFT_MOTOR_EN	11
#define LEFT_MOTOR_I1	7
#define LEFT_MOTOR_I2	8
#define RIGHT_MOTOR_EN	3
#define RIGHT_MOTOR_I1	2//inverser au branchement I1 et I2 car les moteurs sont inversés
#define RIGHT_MOTOR_I2	4

#define SERVO_ABEILLE_ORANGE	9
#define SERVO_ABEILLE_VERT		10
#define SERVO_PORTE				5
#define SERVO_CHATEAU_EAU		6

#define ROBOT_TRIGGER	12

#endif