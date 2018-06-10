#include <Servo.h>

#define LEFT_MOTOR_EN       11
#define LEFT_MOTOR_I1       7
#define LEFT_MOTOR_I2       8

#define RIGHT_MOTOR_EN      3
#define RIGHT_MOTOR_I1      2
#define RIGHT_MOTOR_I2      4

#define SERVO_BEE_ORANGE    9
#define SERVO_BEE_GREEN     10
#define SERVO_DOOR          5
#define SERVO_WATER_CASTLE  6

#define ROBOT_TRIGGER       22
#define COLOR_BUTTON        23
#define FRONT_TOUCH_SENSOR  25

#define GREEN_LED           53
#define RED_LED             52

#define TRIG_LEFT           38
#define ECHO_LEFT           39

#define TRIG_RIGHT          32
#define ECHO_RIGHT          33

#define TRIG_FRONT          36
#define ECHO_FRONT          37

#define PWM_MIN_LEFT 170
#define PWM_MIN_RIGHT 120

#define BACKWARD_CURVE_DISTANCE	400

/**
 * Types
 */

enum RobotState {
    WAIT_FOR_START,
    FOLLOW_FORWARD,
    PUSH_BEE,
    BACKWARD_CURVE,
    FOLLOW_BACKWARD,
    INIT_WALL_FOLLOWER,
    END_MATCH
};

enum Color {
    RED,
    GREEN
};

enum Motor {
    LEFT,
    RIGHT
};

/**
 * Variables
 */

// Communication
char buffer[1024];
int index = 0;
boolean bufferComplete = false;

// Actuators
Servo servoBeeOrange;
Servo servoBeeGreen;

// States
unsigned long timer;
RobotState robotState = WAIT_FOR_START;
Color color;

// Tuning
double kp = 3;

/**
 * Definitions
 */

// Communication
void resetSerial();
void readSerial();
void handleMessage(char message[]); //modifie

// Actuators
void driveMotor(Motor motor, double power_);
void resetServos();
void stopMotors();
void pushBee();

// Sensors
void checkSideColor();
boolean checkRobotTrigger();
boolean checkEndLine();
boolean checkEndMatch();
double getSonarDistance(double maxDistance);
double getOppositeSonarDistance(double maxDistance);

void followWall(double distance = 10, boolean defaultSonar = true);
void initFSM();

/**
 * Implementations
 */

void resetSerial() {
    memset(buffer, 0, 1024);
    index = 0;
    bufferComplete = false;
}

void readSerial() {
    if (Serial.available()) {
        char c = Serial.read();

        buffer[index++] = c;

        if (c == '\n') {
            bufferComplete = true;
            index = 0;

            char message[1024];
            memcpy(message, buffer, 1024);
            handleMessage(message);

            resetSerial();
        }
    }
}
void handleMessage(char message[]) {
   char* args = strtok(message, ":");

    int command = atoi(args);
    args = strtok(0, ":");

    switch (command) {
        case 0x00: {
            Serial.println("reset servos");
            resetServos();
            break;
        }

        case 0x01: {
            Serial.println("stop motors");
            stopMotors();
            break;
        }

        case 0x02: {
            Serial.println("push bee");
            pushBee();
            break;
        }

        case 0x03: {
            Serial.println("check side color");
            checkSideColor();
            Serial.println(color == RED ? "orange" : "green");
            break;
        }

        case 0x04: {
            Serial.println("check robot trigger");
            // white cis male oppresor
            bool triggered = checkRobotTrigger();

            if (triggered) {
                Serial.println("start !");
            } else {
                Serial.println("stop !");
            }

            break;
        }

        case 0x05: {
            Serial.println("check robot front touch sensor");
            bool pushed = checkEndLine();

            if (pushed) {
                Serial.println("pushed !");
            } else {
                Serial.println("non pushed !");
            }

            break;
        }

        case 0x06: {
            Serial.println("move forward");

            int pwmLeft = atoi(args);
            args = strtok(0, ":");

            int pwmRight = atoi(args);
            args = strtok(0, ":");

            driveMotor(LEFT, pwmLeft);
            driveMotor(RIGHT, pwmRight);
            break;
        }
        case 11: {
            Serial.println("move backward");

            int pwmLeft = -atoi(args);
            args = strtok(0, ":");

            int pwmRight = -atoi(args);
            args = strtok(0, ":");

            driveMotor(LEFT, pwmLeft);
            driveMotor(RIGHT, pwmRight);
            break;
        }

        case 0x07: {
            Serial.println("move backward");
            driveMotor(LEFT, -255);
            driveMotor(RIGHT, -255);
            break;
        }

        case 0x08: {
            Serial.println("get sonar distance");
            double distance = getSonarDistance(300);
            Serial.println(distance);
            break;
        }

        case 0x09: {
            Serial.println("follow wall");

            while (1) {
                followWall();
            }
        }

        case 10: {
            Serial.println("get opposite distance");
            double distance = getOppositeSonarDistance(1000);
            Serial.println(distance);
            break;
        }
    }
}

void followWall(double distance, boolean defaultSonar) {
    Motor outerMotor = (color == RED && defaultSonar) ? RIGHT : LEFT;
    Motor innerMotor = (color == RED && defaultSonar) ? LEFT : RIGHT;

    int outerMinimum = (outerMotor == LEFT && defaultSonar) ? PWM_MIN_LEFT : PWM_MIN_RIGHT;
    int innerMinimum = (innerMotor == LEFT && defaultSonar) ? PWM_MIN_LEFT : PWM_MIN_RIGHT;

    double setpoint = distance;
    double measure = getSonarDistance(300);

    // if (measure == -1) {
    //     stopMotors();
    //     return;
    // }

    double error = setpoint - measure;
    double output = kp * error;

    int pwmOuter = map(80 - (output/2.0), 0, 100, outerMinimum, 255);
    int pwmInner = map(80 + (output/2.0), 0, 100, innerMinimum, 255);

    driveMotor(outerMotor, pwmOuter);
    driveMotor(innerMotor, pwmInner);
/*
    Serial.print(measure);
    Serial.print(" ");
    Serial.print(pwmInner);
    Serial.print(" ");
    Serial.println(pwmOuter);*/
    delay(10);
}

void followWallBackward() {
    Motor outerMotor = (color == RED) ? RIGHT : LEFT;
    Motor innerMotor = (color == RED) ? LEFT : RIGHT;

    int outerMinimum = (outerMotor == LEFT) ? PWM_MIN_LEFT : PWM_MIN_RIGHT;
    int innerMinimum = (innerMotor == LEFT) ? PWM_MIN_LEFT : PWM_MIN_RIGHT;

    double setpoint = 10;
    double measure = getSonarDistance(300);

    if (measure == -1) {
        stopMotors();
        return;
    }

    double error = setpoint - measure;
    double output = kp * error;

    int pwmOuter = map(-80 + (output/2.0), 0, -100, -outerMinimum, -255);
    int pwmInner = map(-80 - (output/2.0), 0, -100, -innerMinimum, -255);

    driveMotor(outerMotor, pwmOuter);
    driveMotor(innerMotor, pwmInner);

    Serial.print(measure);
    Serial.print(" ");
    Serial.print(pwmInner);
    Serial.print(" ");
    Serial.println(pwmOuter);
    delay(10);
}

/*void backwardCurve() {
    Motor outerMotor = (color == RED) ? RIGHT : LEFT;
    Motor innerMotor = (color == RED) ? LEFT : RIGHT;

    int outerMinimum = (outerMotor == LEFT) ? PWM_MIN_LEFT : PWM_MIN_RIGHT;
    int innerMinimum = (innerMotor == LEFT) ? PWM_MIN_LEFT : PWM_MIN_RIGHT;

    double setpoint = BACKWARD_CURVE_DISTANCE;
    double measure = getSonarDistance(2000);

    if (measure == -1) {
        stopMotors();
        return;
    }

    double error = setpoint - measure;
    double output = kp * error;

    int pwmOuter = map(-80 + (output/2.0), 0, -100, -outerMinimum, -255);
    int pwmInner = map(-80 - (output/2.0), 0, -100, -innerMinimum, -255);

    driveMotor(outerMotor, pwmOuter);
    driveMotor(innerMotor, pwmInner);


    Serial.print(measure);
    Serial.print(" error : ");
    Serial.print(error);
    Serial.print(" ");
    Serial.print(pwmInner);
    Serial.print(" ");
    Serial.println(pwmOuter);
    delay(10);
}*/

void driveMotor(Motor motor, double power_) {
    double power = constrain(power_, -255, 255);

    int pwmPin  = motor == LEFT ? LEFT_MOTOR_EN : RIGHT_MOTOR_EN;
    int dir1Pin = motor == LEFT ? LEFT_MOTOR_I1 : RIGHT_MOTOR_I1;
    int dir2Pin = motor == LEFT ? LEFT_MOTOR_I2 : RIGHT_MOTOR_I2;

    int dirAVal = power >= 0 ? HIGH : LOW;
    int dirBVal = power >= 0 ? LOW : HIGH;

    digitalWrite(dir1Pin, dirAVal);
    digitalWrite(dir2Pin, dirBVal);
    analogWrite(pwmPin, abs(power));
}

void checkSideColor() {
    if(digitalRead(COLOR_BUTTON) == LOW) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        color = RED;
    } else {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        color = GREEN;
    }
}

void resetServos() {
    servoBeeOrange.write(180);
    servoBeeGreen.write(0);
}

void stopMotors() {
    driveMotor(LEFT, 0);
    driveMotor(RIGHT, 0);
}

void pushBee() {
    if (color == RED) {
        servoBeeOrange.write(0);
    } else {
        servoBeeGreen.write(180);
    }
}

/**
 * Bloking function that return distance in mm from sonar
 * @param maxDistance Maximum intersting distance in mm. Stop waiting once this distance is reached.
 * @return Mesured distance in mm or -1 if over max distance
 */
double getSonarDistance(double maxDistance) {
    int triggerPin = (color == RED) ? TRIG_LEFT : TRIG_RIGHT;
    int echoPin = (color == RED) ? ECHO_LEFT : ECHO_RIGHT;

    double distance = -1;

    // Trigger
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Wait for pulse back
    unsigned long duration = pulseIn(echoPin, HIGH, maxDistance/0.340*2);

    if (duration != 0) {
        distance = duration/2*0.340;
    }

    return distance;
}

double getOppositeSonarDistance(double maxDistance) {
    int triggerPin = (color == GREEN) ? TRIG_LEFT : TRIG_RIGHT;
    int echoPin = (color == GREEN) ? ECHO_LEFT : ECHO_RIGHT;

    double distance = -1;

    // Trigger
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Wait for pulse back
    unsigned long duration = pulseIn(echoPin, HIGH, maxDistance/0.340*2);

    if (duration != 0) {
        distance = duration/2*0.340;
    }

    return distance;
}

void initFSM() {
    robotState = WAIT_FOR_START;
}

boolean checkRobotTrigger() {
    // input pullup, triggered on LOW
    if (digitalRead(ROBOT_TRIGGER) == LOW) {
        return true;
    }

    return false;
}

boolean checkEndMatch() {
    if (millis() - timer >= 100*1000) {
        return true;
    }

    return false;
}

boolean checkEndLine() {
    // input pullup, triggered on LOW
    if (digitalRead(FRONT_TOUCH_SENSOR) == LOW) {
        return true;
    }

    return false;
}

void setup() {
    // serial
    Serial.begin(115200);
    resetSerial();
    Serial.println("setup");

    // Init pinout
    pinMode(LEFT_MOTOR_EN, OUTPUT);
    pinMode(LEFT_MOTOR_I1, OUTPUT);
    pinMode(LEFT_MOTOR_I2, OUTPUT);

    pinMode(RIGHT_MOTOR_EN, OUTPUT);
    pinMode(RIGHT_MOTOR_I1, OUTPUT);
    pinMode(RIGHT_MOTOR_I2, OUTPUT);

    digitalWrite(LEFT_MOTOR_I1, HIGH);
    digitalWrite(LEFT_MOTOR_I2, LOW);
    analogWrite(LEFT_MOTOR_EN, 150);

    pinMode(SERVO_BEE_ORANGE, OUTPUT);
    pinMode(SERVO_BEE_GREEN, OUTPUT);
    pinMode(SERVO_DOOR, OUTPUT);
    pinMode(SERVO_WATER_CASTLE, OUTPUT);

    pinMode(ROBOT_TRIGGER, INPUT_PULLUP);
    pinMode(COLOR_BUTTON, INPUT);

    pinMode(FRONT_TOUCH_SENSOR, INPUT_PULLUP);

    pinMode(ECHO_LEFT, INPUT);
    pinMode(TRIG_LEFT, OUTPUT);
    pinMode(ECHO_RIGHT, INPUT);
    pinMode(TRIG_RIGHT, OUTPUT);

    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    // Init servos
    servoBeeOrange.attach(SERVO_BEE_ORANGE);
    servoBeeGreen.attach(SERVO_BEE_GREEN);
    resetServos();

    // Init motors
    stopMotors();

    // Init FSM
    initFSM();
    Serial.println("FSM: WAIT_FOR_START");
}

void loop() {
    // Get Serial commands
    readSerial();

    // FSM
    if (robotState != WAIT_FOR_START && checkEndMatch()) {
        Serial.println("FSM: END_MATCH");
        robotState = END_MATCH;
    }

    switch (robotState) {
        case WAIT_FOR_START: {
            // Get side color
            checkSideColor();

            if (checkRobotTrigger()) {
                Serial.println("FSM: FOLLOW_FORWARD");
                robotState = FOLLOW_FORWARD;
                timer = millis();
            }

            break;
        }

        case FOLLOW_FORWARD: {
            followWall();

            if (checkEndLine()) {
                Serial.println("FSM: PUSH_BEE");
                robotState = PUSH_BEE;
            }

            break;
        }

        case PUSH_BEE: {
            pushBee();
            delay(2000);

            Serial.println("el mumuxe FSM: BACKWARD_CURVE");
            //robotState = BACKWARD_CURVE;
            //robotState = FOLLOW_BACKWARD;
            robotState = BACKWARD_CURVE;




        	Motor outerMotor = (color == RED) ? RIGHT : LEFT;
    		Motor innerMotor = (color == RED) ? LEFT : RIGHT;
   			int outerMinimum = (outerMotor == LEFT) ? PWM_MIN_LEFT : PWM_MIN_RIGHT;
  			int innerMinimum = (innerMotor == LEFT) ? PWM_MIN_LEFT : PWM_MIN_RIGHT;
    		driveMotor(outerMotor, -map(0, 0, 100, outerMinimum, 255));
    		driveMotor(innerMotor, -map(100, 0, 100, innerMinimum, 255));
    		delay(3500);

            break;
        }

        /*case BACKWARD_CURVE: {
        	backwardCurve();

        	Serial.println("BACKWARD_CURVE");
        	if(abs(BACKWARD_CURVE_DISTANCE - getSonarDistance(2000)) < 20){
            	robotState = END_MATCH;        		
    		}

        	break;
        }*/

        /*case FOLLOW_BACKWARD: {
        	followWallBackward();

        	Serial.println("FOLLOW_BACKWARD");
        	if(getSonar2Distance(2000) > 800){
            	robotState = END_MATCH;
            	delay(1000);
        	}

        	break;
        }*/

        case BACKWARD_CURVE: {
    		resetServos();
        	
        	followWall(30, false);

            break;
        }

        case INIT_WALL_FOLLOWER: {
        	Motor outerMotor = (color == RED) ? RIGHT : LEFT;
    		Motor innerMotor = (color == RED) ? LEFT : RIGHT;

        	driveMotor(outerMotor, 200);
    		driveMotor(innerMotor, -255);

    		//delay(2000);
    		double sonarDistance = getSonarDistance(2000);
    		if ((sonarDistance < 10) && (sonarDistance != -1))
    		{
            	robotState = END_MATCH;    			
    		}
        	break;
        }

        case END_MATCH: {
            stopMotors();
        	Serial.println("END_MATCH");
        	break;
        }
    }
}
