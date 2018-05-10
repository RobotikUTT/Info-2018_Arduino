#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "Arduino.h"

#define LEFT_FOREWARD_SENS		1
#define RIGHT_FOREWARD_SENS		1

#define LIGHT_THRESHOLD_WHITE 125//180 //entre les 2 gris
#define LIGHT_THRESHOLD_BLACK 110//160 //les 2 servent a avoir une marge de securite

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

#define SENSITIVITY						90	//sensibilite du suiveur de ligne (~reactivite) (pour direction en numerique)
#define ANALOG_SENSITIVITY				48
#define PWM_SPEED						110 //<=255
#define PWM_CROSSROADS_SPEED			170 //<= 255
#define PWM_CROSSROADS_INVERSE_SPEED	0 //155 //<= 255
#define MAX_PWM_LINE_DIFFERENCE			120//50//35
#define LUMINOSITY_THRESHOLD_STARTING_HELP	5 //seuil ou valeur luminosite comprise (ex : -5 a 5 de difference) ou on detecte que le robot demarre pas
#define MAX_PHOTO_LUMINOSITY			450 //luminosite max que la photo au centre peut avoir (valeur approchee mais INFERIEURE a la valeur reelle)


#endif