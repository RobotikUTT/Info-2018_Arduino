#undef abs
#include <vector>
#include "pins.h"
#include "parameters.h"
#include <MotorControl.h>
#include <Motor.h>
#include <Servo.h>
#include <LineFollower.h>
#include <LineWatcher.h>
//#include "HC_SR04.h"
#include "SonarArray.h"
//standart c++ :
//#include <StandardCplusplus.h>
#include <NewPing.h>



Color color;


Servo servoAbeilleOrange;
Servo servoAbeilleVert;
Servo servoPorte;
Servo servoChateauEau;

uint8_t robotState = 0; //0: robot off | 1: robot on  //a laisser avant motorcontrol

LineWatcher watcher(LIGHT_THRESHOLD_WHITE, LIGHT_THRESHOLD_BLACK, PHOTO_0, PHOTO_1, PHOTO_2, PHOTO_3, PHOTO_4, PHOTO_0_ERROR_M, PHOTO_1_ERROR_M, PHOTO_2_ERROR_M, PHOTO_3_ERROR_M, PHOTO_4_ERROR_M, PHOTO_0_ERROR_B, PHOTO_1_ERROR_B, PHOTO_2_ERROR_B, PHOTO_3_ERROR_B, PHOTO_4_ERROR_B);//enregistrement des pins

Motor leftMotor(LEFT_MOTOR_EN, LEFT_MOTOR_I1, LEFT_MOTOR_I2, LEFT_FOREWARD_SENS);
Motor rightMotor(RIGHT_MOTOR_EN, RIGHT_MOTOR_I1, RIGHT_MOTOR_I2, RIGHT_FOREWARD_SENS);

MotorControl motor(&leftMotor, &rightMotor, &watcher, SENSITIVITY, ANALOG_SENSITIVITY, PWM_SPEED, PWM_CROSSROADS_SPEED, PWM_CROSSROADS_INVERSE_SPEED, MAX_PWM_LINE_DIFFERENCE, &robotState);

LineFollower follower(&watcher, &motor, MAX_PHOTO_LUMINOSITY, &color);

uint8_t robotTriggerMemory = HIGH;
uint8_t e; //var pour trigger (ou bouton si existant)


//SonarArray sonarArray((uint8_t)1, 38, 39);//trig, echo
//HC_SR04 sonarAvant(36, 37); //38, 39
//HC_SR04 sonar(38, 39);
uint32_t sonarMillis = millis();

uint32_t timerStop = millis();

NewPing sonarAvant(36, 37, 100); // NewPing setup of pins and maximum distance.

uint8_t sonarVar;

CrossroadsDirection fifo[2] = {RIGHT, RIGHT};
uint8_t crossroads_number = 0;


void setup()
{
	Serial.begin(57600);
	servoAbeilleOrange.attach(SERVO_ABEILLE_ORANGE);
	servoAbeilleVert.attach(SERVO_ABEILLE_VERT);
	servoPorte.attach(SERVO_PORTE);
	servoChateauEau.attach(SERVO_CHATEAU_EAU);
	pinMode(ROBOT_TRIGGER, INPUT_PULLUP);
	pinMode(COLOR_BUTTON, INPUT);
	if(digitalRead(COLOR_BUTTON) == HIGH)
	{
		digitalWrite(RED_LED, HIGH);
		digitalWrite(GREEN_LED, LOW);
		color = RED;
	}
	else
	{
		digitalWrite(RED_LED, LOW);
		digitalWrite(GREEN_LED, HIGH);
		color = GREEN;
	}

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
	    timerStop = millis();
	}
	robotTriggerMemory = e;

	Serial.print("robotState");
	Serial.print(robotState);

	follower.update(robotState);

	motor.update();


	if (millis() - sonarMillis > 200) //event pour tester obstacles
	{
 		sonarVar = sonarAvant.ping_cm();
		Serial.print("sonar: dist: ");
		//Serial.println(sonar.getDistance());
		Serial.print(sonarVar);

		/*Serial.print("Ping: ");
 		Serial.print(sonarAvant.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
 		Serial.println("cm");*/

 		if((sonarVar < 15) && (sonarVar != 0) /*&& (follower.getCrossroadsNb() < 1)*/)
 		{
 		  	motor.motorState(0);
 		  	while(sonarVar < 15){
 				sonarVar = sonarAvant.ping_cm();
 		  	}
 		}

		sonarMillis = millis();
	}
	// sonarArray.update();


	// std::vector<uint8_t> sonar_dist = sonarArray.getNormalizedDistances();
	// if(sonar_dist[0] < 10){
	//      while(1){
	//          motor.motorState(0);
	//          Serial.println(sonar_dist[0]);
	//          sonarArray.update();

	// std::vector<uint8_t> sonar_dist = sonarArray.getNormalizedDistances();
	//      }
	//  } 


	Serial.print("  *  ");
	Serial.print(robotState);
	Serial.println();
	//if(1 == 1/*digitalRead(ROBOT_TRIGGER) == HIGH*/)
	if(1 == 1/*digitalRead(ROBOT_TRIGGER) == HIGH*/)
	{
		servoAbeilleVert.write(5);
		servoAbeilleOrange.write(180);
		servoPorte.write(5);
	}
	else
	{
		if(color == GREEN)
		{
			servoAbeilleVert.write(90);
			servoAbeilleOrange.write(180);
		}
		else
		{
			servoAbeilleVert.write(5);
			servoAbeilleOrange.write(90);
		}
		servoPorte.write(90);
	}
	//pour ranger les servo
	//servoAbeilleVert.write(0);
	//servoAbeilleOrange.write(180);
	//servoPorte.write(5); //min 5 !!!
	servoChateauEau.write(0); //mettre a 45 pour ouvrir | ferme a 0
	//servoPorte.write(110);
	/*delay(5000);
	servoChateauEau.write(45);
	delay(5000);
	servoPorte.write(90);*/



	if(millis() - timerStop > 95000){
	   while(1){
	        	motor.motorState(0);//remplacer par la fonction stop
 			} 
	}

	if (follower.getNextCrossroads() == UNKNOWN)
	{
		follower.addNextCrossroads(fifo[crossroads_number]);
		crossroads_number++;
		Serial.print("cr nb : ");
		Serial.print(crossroads_number);
		Serial.print("  ");
	}
}