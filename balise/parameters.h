#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "Arduino.h"

#define BUTTON_DELAY	2000

#define ANGLE			50

#define MIN_DELAY		4000	//1000 //pour regler vitesse moteur

#define NUMBER_OBJECT_2_MEASURE		4	//nombre de fois des balises a mesurer pour valider la calibration, un nombre pair est mieux pour limiter les erreurs

//parametres de la regression lineaire : ro = a*delta_teta+b
#define LIN_REG_A		-66.667
#define LIN_REG_B		1666.7
#define P 				1.3

#define PI_100			0.0314159	//pi/100 pour convertir les pas en radian

#endif