#include "pins.h"
#include "parameters.h"
#include <MotorControl.h>
#include <Motor.h>
#include <Servo.h>
#include <LineFollower.h>
#include <LineWatcher.h>

Servo servoAbeilleOrange;
Servo servoAbeilleVert;
Servo servoPorte;
Servo servoChateauEau;

LineWatcher watcher(LIGHT_THRESHOLD_WHITE, LIGHT_THRESHOLD_BLACK, PHOTO_0, PHOTO_1, PHOTO_2, PHOTO_3, PHOTO_4, PHOTO_0_ERROR_M, PHOTO_1_ERROR_M, PHOTO_2_ERROR_M, PHOTO_3_ERROR_M, PHOTO_4_ERROR_M, PHOTO_0_ERROR_B, PHOTO_1_ERROR_B, PHOTO_2_ERROR_B, PHOTO_3_ERROR_B, PHOTO_4_ERROR_B);//enregistrement des pins

Motor leftMotor(LEFT_MOTOR_EN, LEFT_MOTOR_I1, LEFT_MOTOR_I2, LEFT_FOREWARD_SENS);
Motor rightMotor(RIGHT_MOTOR_EN, RIGHT_MOTOR_I1, RIGHT_MOTOR_I2, RIGHT_FOREWARD_SENS);

MotorControl motor(&leftMotor, &rightMotor, SENSITIVITY, ANALOG_SENSITIVITY, PWM_SPEED, PWM_CROSSROADS_SPEED, PWM_CROSSROADS_INVERSE_SPEED, MAX_PWM_LINE_DIFFERENCE);

LineFollower follower(&watcher, &motor, MAX_PHOTO_LUMINOSITY);

uint8_t robotTriggerMemory = HIGH;
uint8_t e; //var pour trigger (ou bouton si existant)

uint8_t robotState = 0; //0: robot off | 1: robot on

void setup()
{
	//TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
	//TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
	//TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
	  TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
	//TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
	//TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
	//TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
	Serial.begin(9600);
	servoAbeilleOrange.attach(SERVO_ABEILLE_ORANGE);
	servoAbeilleVert.attach(SERVO_ABEILLE_VERT);
	servoPorte.attach(SERVO_PORTE);
	servoChateauEau.attach(SERVO_CHATEAU_EAU);
	pinMode(ROBOT_TRIGGER, INPUT);
}

void loop()
{
	for(int i = 4 ; i >= 0 ; i--)
	{
		//Serial.print(watcher.photoState(i));
		//Serial.print("  /  ");
		Serial.print(watcher.photoVal(i));
		Serial.print("  |  ");
	}

	e = digitalRead(ROBOT_TRIGGER);
	if((e == LOW) && (e != robotTriggerMemory)){
	    robotState = 1;
	}
	robotTriggerMemory = e;

	follower.update(robotState);
	

	motor.update();

	Serial.print("  *  ");
	Serial.print(robotState);
	Serial.println();
	servoAbeilleVert.write(0);
	servoAbeilleOrange.write(180);
	servoPorte.write(5); //min 5 !!!
	servoChateauEau.write(0);
	//servoPorte.write(110);
}