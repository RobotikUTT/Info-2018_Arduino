#include "pins.h"
//#include <MotorControl.h>
#include <Servo.h>
#include <lineFollower.h>

Servo servoAbeilleOrange;
Servo servoAbeilleVert;

void setup()
{
	servoAbeilleOrange.attach(SERVO_ABEILLE_ORANGE);
	servoAbeilleVert.attach(SERVO_ABEILLE_VERT);
}

void loop()
{

}