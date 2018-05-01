#include "pins.h"
#include "parameters.h"
#include <MotorControl.h>
#include <Motor.h>
#include <Servo.h>
#include <LineFollower.h>
#include <LineWatcher.h>

Servo servoAbeilleOrange;
Servo servoAbeilleVert;

Motor leftMotor(LEFT_MOTOR_EN, LEFT_MOTOR_I1, LEFT_MOTOR_I2, LEFT_FOREWARD_SENS);
Motor rightMotor(RIGHT_MOTOR_EN, RIGHT_MOTOR_I1, RIGHT_MOTOR_I2, RIGHT_FOREWARD_SENS);

MotorControl motor(&leftMotor, &rightMotor);

LineWatcher watcher(LIGHT_THRESHOLD_WHITE, LIGHT_THRESHOLD_BLACK, PHOTO_0, PHOTO_1, PHOTO_2, PHOTO_3, PHOTO_4, PHOTO_0_ERROR_M, PHOTO_1_ERROR_M, PHOTO_2_ERROR_M, PHOTO_3_ERROR_M, PHOTO_4_ERROR_M, PHOTO_0_ERROR_B, PHOTO_1_ERROR_B, PHOTO_2_ERROR_B, PHOTO_3_ERROR_B, PHOTO_4_ERROR_B);//enregistrement des pins


void setup()
{
	Serial.begin(9600);
	servoAbeilleOrange.attach(SERVO_ABEILLE_ORANGE);
	servoAbeilleVert.attach(SERVO_ABEILLE_VERT);
}

void loop()
{
	for(int i = 0 ; i <= 4 ; i++)
	{
		Serial.print(watcher.photoState(i));
		Serial.print("  |  ");
	}
	Serial.println();
	delay(200);
}