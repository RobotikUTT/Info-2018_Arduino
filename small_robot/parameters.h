#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "Arduino.h"

#define LEFT_FOREWARD_SENS		1
#define RIGHT_FOREWARD_SENS		1

#define LIGHT_THRESHOLD_WHITE 300
#define LIGHT_THRESHOLD_BLACK 300

//Regression lineaire : y = m*x+b

#define PHOTO_0_ERROR_M		1.154
#define PHOTO_1_ERROR_M		1		//capteur reference
#define PHOTO_2_ERROR_M		1.117
#define PHOTO_3_ERROR_M		0.974
#define PHOTO_4_ERROR_M		1.898

#define PHOTO_0_ERROR_B		-3.4
#define PHOTO_1_ERROR_B		0		//capteur reference
#define PHOTO_2_ERROR_B		-5.1
#define PHOTO_3_ERROR_B		-10.0
#define PHOTO_4_ERROR_B		-5.9

#define SENSITIVITY			90	//sensibilite du suiveur de ligne (~reactivite) (pour direction en numerique)
#define ANALOG_SENSITIVITY	30	//>4
#define PWM_SPEED			130


#endif