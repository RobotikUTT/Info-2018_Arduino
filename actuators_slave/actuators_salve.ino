#include "pins.h"
#include "protocol.h"
#include "HC_SR04.h"
#include "SonarArray.h"
#include <vector>
// this arduino is supposed to be pluged on the can bus. I communicates
// with the other arduino on the board through the hardware serial.
// It has to read the data from the other arduino and send it on the CAN bus.
// this arduino controls the pliers (a stepper, an AX12 and a limit switch)
// it also controls a L298 double H-bridge (aka the ball thrower)

#define SONAR_DELAY	            100
#define EMERGENCY_SONAR_DELAY   SONAR_DELAY

void readMaster();
void writeMaster(uint8_t* message, uint8_t size);

SonarArray sonarArray((uint8_t)5,SONAR_FRONT_L_TRIG,SONAR_FRONT_L_ECHO,
			 SONAR_FRONT_R_TRIG,SONAR_FRONT_R_ECHO,
			 SONAR_L_TRIG,SONAR_L_ECHO,
			 SONAR_R_TRIG,SONAR_R_ECHO,
             SONAR_BACK_TRIG,SONAR_BACK_ECHO);

void decodeFrame(uint8_t* message);

uint32_t sonarMillis = millis();
void setup()
{
	Serial.begin(BAUDRATE);
}

void loop()
{
	sonarArray.update();

    uint32_t sonar_delay;
    if( sonarArray.detectTooClose() )
    {
        sonar_delay = EMERGENCY_SONAR_DELAY;
    }
    else
    {
        sonar_delay = SONAR_DELAY;
    }
	if (millis() - sonarMillis > sonar_delay)
	{
        uint8_t sonarFrame[MSG_SIZE];

        std::vector<uint8_t> sonar_dist = sonarArray.getNormalizedDistances();

        uint8_t size = encodeFrame(sonarFrame, SONAR_DISTANCE, 
            sonar_dist[0], sonar_dist[1],sonar_dist[2],
            sonar_dist[3], sonar_dist[4]);
        sonarArray.disableAll();
        writeMaster(sonarFrame, size);
        // std::vector<uint16_t>::iterator it;
        // for (it = sonar_dist.begin(); it != sonar_dist.end(); it++)
        // {
        //     Serial.print(*it);
        //     Serial.print("|");
        // }
        // Serial.println();
        sonarArray.enableAll();
	
		sonarMillis = millis();
	}
}

void decodeFrame(uint8_t* message)
{
    uint8_t mode = message[0];
    switch (mode) 
    {
        case SET_SERVO :
        {
            uint8_t sonarID = message[1];
            uint8_t angle = message[2];
            // send order to servo
            break;
        }
        default:
            break;

    }
}

void readMaster()
{
    if (Serial.available() > 0)
    {
        uint8_t buffer_in[MSG_SIZE];
        Serial.readBytesUntil('\n',(char*)buffer_in, MSG_SIZE);
        decodeFrame(buffer_in);
    }
}

void writeMaster(uint8_t* message, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Serial.write(message[i]);
    }

    Serial.print('\n');
}