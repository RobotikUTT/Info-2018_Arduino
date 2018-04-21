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

SonarArray sonarArray((uint8_t)4,SONAR_1_TRIG,SONAR_1_ECHO,
			 SONAR_2_TRIG,SONAR_2_ECHO,
			 SONAR_3_TRIG,SONAR_3_ECHO,
			 SONAR_4_TRIG,SONAR_4_ECHO);

// void updateCan (void);
void decodeFrame(uint8_t* message);

void getCan (void);
//PololuA4983 stepper(STEP_PIN,DIR_PIN,EN_PIN);
//Pliers pliers(&stepper, PLIERS_LIMIT_SWITCH_PIN);

// HC_SR04 sonar1(SONAR_1_TRIG, SONAR_1_ECHO);
// HC_SR04 sonar2(SONAR_2_TRIG, SONAR_2_ECHO);
// HC_SR04 sonar3(SONAR_3_TRIG, SONAR_3_ECHO);
// HC_SR04 sonar4(SONAR_4_TRIG, SONAR_4_ECHO);
//MCP2515 mcp2515(SPI_SS_PIN);

struct can_frame canRxMsg;  //strcture de la trame CAN reçue
//struct can_frame canTxMsg; //trame CAN à envoyer


uint32_t orderMillis = millis();
void setup()
{
	Serial.begin(115200);
	Serial.println("Hello World!");

	//pliers.reset();
	//sonar1.enable();
}

void loop()
{
	//pliers.update();
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

void getCan (void){
  	MCP2515::ERROR can_error = mcp2515.readMessage(&canRxMsg);
	if (canRxMsg.can_id == ARDUINO_CAN_ADDR)
	{
		if (( can_error == MCP2515::ERROR_OK)) //Si le CAN est disponible
		{ 
		
		decodeFrame(canRxMsg.data);		
		}
		else if (can_error == MCP2515::ERROR_FAIL) {
		//Serial.println("received stuff but failed");
		}
	}
}

void decodeFrame(uint8_t* message)
{
    uint8_t mode = message[0];
    switch (mode) 
    {
        case HANDSHAKE:
        {           
        	CanSender::canSend(WHOAMI,ARDUINO_CAN_ADDR);
            break;
        }

        case SET_MODE:
        {
        	CanSender::canSend(ORDER_COMPLETED,SET_MODE);
            break;
        }

        

        case MOVE_PLIERS :
        {
            unsigned char level;
            level = message[1];

            break;
        }

        case CLOSE_OPEN_PLIERS :
        {
            unsigned char order;
            order = message[1];
            if ( order > 0 )
            {

            }
            else
            {

            }
            
            
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