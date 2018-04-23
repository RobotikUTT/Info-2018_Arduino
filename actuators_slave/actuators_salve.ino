#include "pins.h"
#include <SPI.h>
#include <mcp2515.h>
#include "protocol.h"
#include "PololuA4983.h"
#include "Pliers.h"
#include "HC_SR04.h"
#include "SonarArray.h"
#include "canSender.h"
#include <vector>
// this arduino is supposed to be pluged on the can bus. I communicates
// with the other arduino on the board through the hardware serial.
// It has to read the data from the other arduino and send it on the CAN bus.
// this arduino controls the pliers (a stepper, an AX12 and a limit switch)
// it also controls a L298 double H-bridge (aka the ball thrower)

#define SERIAL_DELAY	100

SonarArray sonarArray((uint8_t)4,SONAR_FRONT_L_TRIG,SONAR_FRONT_L_ECHO,
			 SONAR_FRONT_R_TRIG,SONAR_FRONT_R_ECHO,
			 SONAR_L_TRIG,SONAR_L_ECHO,
			 SONAR_R_TRIG,SONAR_R_ECHO,
             SONAR_BACK_TRIG,SONAR_BACK_ECHO);

void decodeFrame(uint8_t* message);

uint32_t orderMillis = millis();
void setup()
{
	Serial.begin(115200);
	Serial.println("Hello World!");

}

void loop()
{
	sonarArray.update();

	if (millis() - orderMillis > SERIAL_DELAY)
	{
		sonarArray.disableAll();
		std::vector<uint16_t> sonar_dist = sonarArray.getDistances();

		std::vector<uint16_t>::iterator it;
		for (it = sonar_dist.begin(); it != sonar_dist.end(); it++)
		{
			Serial.print(*it);
			Serial.print("|");
		}
		Serial.println();
		sonarArray.enableAll();
		orderMillis = millis();
	}
}

void decodeFrame(uint8_t* message)
{
    uint8_t mode = message[0];
    switch (mode) 
    {
        case HANDSHAKE:
        {           
            break;
        }

        case SET_MODE:
        {
            break;
        }

        

        case MOVE_PLIERS :
        {
            break;
        }

        case CLOSE_OPEN_PLIERS :
        {
            break;
        }

        case THROW_BALLS :
        {
            break;
        }
        default:
            break;

    }
}