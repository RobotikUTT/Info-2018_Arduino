#include "pins.h"
#include <SPI.h>
#include <mcp2515.h>
#include "protocol.h"
#include "PololuA4983.h"
#include "Pliers.h"
#include "canSender.h"
#include "MotorControl.h"

//#include <vector>
// this arduino is supposed to be pluged on the can bus. I communicates
// with the other arduino on the board through the hardware serial.
// It has to read the data from the other arduino and send it on the CAN bus.
// this arduino controls the pliers (a stepper, an AX12 and a limit switch)
// it also controls a L298 double H-bridge (aka the ball thrower)


void decodeFrame(uint8_t* message);

void getCan (void);
void readSlave();
void writeSlave(uint8_t* message,  uint8_t size);

MotorControl ballThrower();
PololuA4983 stepper(STEPPER_A_STEP,STEPPER_A_DIR);
Pliers pliers(&stepper, PLIERS_LIMIT_SWITCH_PIN);


MCP2515 mcp2515(SPI_SS_PIN);

struct can_frame canRxMsg;  //strcture de la trame CAN reçue
struct can_frame canTxMsg; //trame CAN à envoyer


uint32_t orderMillis = millis();
void setup()
{
	Serial.begin(BAUDRATE);

    canTxMsg.can_id  = BBB_CAN_ADDR;  //L'adresse (de destination) est mise à 0x62 car l'arduino qui va recevoir la trame est bloquée à cette adresse (elle n'a pas de switchs)
    canTxMsg.can_dlc = 8; //payload toujours 8
    SPI.begin();
    mcp2515.reset();
    mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);  //paramètres les plus rapides testés pour le CAN
    mcp2515.setNormalMode();


    pliers.begin();
}

void loop()
{
	pliers.update();

    getCan();
    readSlave();
	CanSender::canSendTask();

}

void getCan (void){
  	MCP2515::ERROR can_error = mcp2515.readMessage(&canRxMsg);
	if ( canRxMsg.can_id == ARDUINO_CAN_ADDR || 
         canRxMsg.can_id == ALL_CAN_ADDR )
	{
		if (( can_error == MCP2515::ERROR_OK)) //Si le CAN est disponible
		{ 
            decodeFrame(canRxMsg.data);		
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
            Serial.println("HANDSHAKE");
            uint8_t answer[MSG_SIZE];
            encodeFrame(answer, WHOAMI,ARDUINO_CAN_ADDR);
        	CanSender::canSend(answer);
            break;
        }

        case SET_MODE:
        {
            break;
        }

        

        case MOVE_PLIERS :
        {
            Serial.println("MOVE PLIERS");
            unsigned char level;
            level = message[1];
            pliers.setLevel(level);
            break;
        }

        case CLOSE_OPEN_PLIERS :
        {
            Serial.println("CLOSE OPEN PLIERS");
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
            Serial.println("THROW_BALLS");
            
            break;
        }

        case SONAR_DISTANCE:
        {
            uint8_t answer[MSG_SIZE];

            //int conversion for variadic arguments function
            int dist0 = message[1];
            int dist1 = message[2];
            int dist2 = message[3];
            int dist3 = message[4];
            int dist4 = message[5];
            encodeFrame(answer, 
                SONAR_DISTANCE, dist0, dist1, dist2, dist3, dist4);
            CanSender::canSend(answer);
            break;
        }

        case SET_SERVO:
        {
            uint8_t answer[MSG_SIZE];
            uint8_t sonarID = message[1];
            uint8_t angle   = message[2];
            uint8_t size = encodeFrame(answer, SET_SERVO, sonarID, angle);
            writeSlave(answer,size);


            break;
        }
        case ACTION_PLIERS:
        {
            uint8_t action, level;

            action = message[1];
            level  = message[2];

            cube_action_t cubeAction;

            if (action == PLIERS_TAKE)
            {
                cubeAction = TAKE;
            }
            else if (action == PLIERS_RELEASE)
            {
                cubeAction = RELEASE;
            }

            pliers.addAction(cubeAction, level);

        }

        default:
            break;

    }
}
void readSlave()
{
    // Management of the slave arduino
    if (Serial.available() > 0)
    {
        uint8_t buffer_in[MSG_SIZE];
        Serial.readBytesUntil('\n',(char*)buffer_in, MSG_SIZE);
        decodeFrame(buffer_in);
    }
}

void writeSlave(uint8_t* message, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Serial.write(message[i]);
    }

    Serial.print('\n');
}

