#include "pins.h"
#include "parameters.h"
#include <MotorControl.h>
#include <Servo.h>
#include <lineFollower.h>

Servo servoAbeilleOrange;
Servo servoAbeilleVert;

MotorControl motorGauche(MOTOR_GAUCHE_EN, MOTOR_GAUCHE_I1, MOTOR_GAUCHE_I2, FOREWARD_SENS_GAUCHE);
MotorControl motorDroit(MOTOR_DROIT_EN, MOTOR_DROIT_I1, MOTOR_DROIT_I2, FOREWARD_SENS_DROIT);

lineFollower (uint16_t light_threshold ,uint8_t photo_0_pin, uint8_t photo_1_pin, uint8_t photo_2_pin, uint8_t photo_3_pin, uint8_t photo_4_pin, uint16_t photo_0_error, uint16_t photo_1_error, uint16_t photo_2_error, uint16_t photo_3_error, uint16_t photo_4_error);//enregistrement des pins


void setup()
{
	servoAbeilleOrange.attach(SERVO_ABEILLE_ORANGE);
	servoAbeilleVert.attach(SERVO_ABEILLE_VERT);
}

void loop()
{
	
}