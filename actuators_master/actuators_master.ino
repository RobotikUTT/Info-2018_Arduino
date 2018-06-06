#include "pins.h"
#include <SPI.h>
#include <mcp2515.h>
#include "protocol.h"
#include "PololuA4983.h"
#include "Pliers.h"
#include "canSender.h"
#include "MotorControl.h"
#include "HC_SR04.h"
#include "SonarArray.h"
#include <vector>


// this arduino is supposed to be pluged on the can bus. I communicates
// with the other arduino on the board through the hardware serial.
// It has to read the data from the other arduino and send it on the CAN bus.
// this arduino controls the pliers (a stepper, an AX12 and a limit switch)
// it also controls a L298 double H-bridge (aka the ball thrower)

#define SONAR_DELAY             100
#define EMERGENCY_SONAR_DELAY   SONAR_DELAY

void decodeFrame(uint8_t* message);
void handleSonars();

void getCan (void);

MotorControl* ballThrower;
//MotorControl* motor2;
PololuA4983 stepper(STEPPER_STEP,STEPPER_DIR);

SonarArray sonarArray((uint8_t)5,SONAR_FRONT_L_TRIG,SONAR_FRONT_L_ECHO,
             SONAR_FRONT_R_TRIG,SONAR_FRONT_R_ECHO, 
             SONAR_BACK_L_TRIG,SONAR_BACK_L_ECHO, 
             SONAR_BACK_R_TRIG, SONAR_BACK_R_ECHO,
             SONAR_BACK_TRIG, SONAR_BACK_ECHO);

Pliers* pliers;


MCP2515* mcp2515;

struct can_frame canRxMsg;  //strcture de la trame CAN reçue
struct can_frame canTxMsg; //trame CAN à envoyer


uint32_t orderMillis = millis();
uint32_t sonarMillis = millis();
void setup()
{
	Serial.begin(57600);
    SPI.begin();
    Serial.println("begin");
    canTxMsg.can_id  = BBB_CAN_ADDR;  //L'adresse (de destination) est mise à 0x62 car l'arduino qui va recevoir la trame est bloquée à cette adresse (elle n'a pas de switchs)
    canTxMsg.can_dlc = 8; //payload toujours 8
    mcp2515 = new MCP2515(SPI_SS_PIN);
    MCP2515::ERROR can_error;
    mcp2515->reset();

    mcp2515->setBitrate(CAN_500KBPS, MCP_16MHZ);  //paramètres les plus rapides testés pour le CAN
    can_error = mcp2515->setNormalMode();

    ballThrower = new MotorControl(MOTOR_A_EN,MOTOR_A_IN_1, MOTOR_A_IN_2,HIGH);
    //ballThrower->setSpeed(255);

    pliers = new Pliers(&stepper, PLIERS_LIMIT_SWITCH_PIN);
    

    pliers->begin();
    
    //cube_action_t action = TAKE;

    
    //pliers->addAction(TAKE,0);
    //pliers->setLevel(1);
    //pliers->setLevel(0);
    // pliers->setLevel(3);
    // pliers->setLevel(3);
    // pliers->addAction(TAKE,0);
    // pliers->addAction(TAKE,0);
    // pliers->addAction(TAKE,0);
    // pliers->addAction(TAKE,0);
    // pliers->open();
    // delay(1000);
    // pliers->close();
    // delay(1000);
    // pliers->open();
    // delay(1000);
    // pliers->close();
    // delay(500);
    // pliers->open();
    // delay(500);
    // pliers->close();
    // delay(500);
    // pliers->open();
    // delay(500);
    // pliers->close();
    // delay(500);


}

void loop()
{
    handleSonars();
    sonarArray.update();
    pliers->update();

    getCan();
    CanSender::canSendTask();  

}

void getCan (void){
  	MCP2515::ERROR can_error = mcp2515->readMessage(&canRxMsg);
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
            //Serial.println("HANDSHAKE");
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
            //DEPRECATED
            Serial.println("MOVE PLIERS");
            unsigned char level;
            level = message[1];
            cube_action_t action = SET_LEVEL;
            pliers->addAction(action, level);
            break;
        }

        case CLOSE_OPEN_PLIERS :
        {
            // DEPRECATED
            Serial.println("CLOSE OPEN PLIERS");
            unsigned char order;
            order = message[1];
            
            
            
            break;
        }

        case THROW_BALLS :
        {
            //Serial.println("THROW_BALLS");
            
            break;
        }

        // case SONAR_DISTANCE:
        // {
        //     uint8_t answer[MSG_SIZE];

        //     //int conversion for variadic arguments function
        //     int dist0 = message[1];
        //     int dist1 = message[2];
        //     int dist2 = message[3];
        //     int dist3 = message[4];
        //     int dist4 = message[5];
        //     encodeFrame(answer, 
        //         SONAR_DISTANCE, dist0, dist1, dist2, dist3, dist4);
        //     CanSender::canSend(answer);
        //     break;
        // }

        case SET_SERVO:
        {
            uint8_t answer[MSG_SIZE];
            uint8_t sonarID = message[1];
            uint8_t angle   = message[2];
            //uint8_t size = encodeFrame(answer, SET_SERVO, sonarID, angle);
            //servo handeling

            break;
        }
        case ACTION_PLIERS:
        {
            Serial.print("action pliers|");
            uint8_t action, level;

            action = message[1];
            level  = message[2];

            Serial.print(action);
            Serial.print("|");
            Serial.println(level);

            

            switch(action)
            {
                case PLIERS_TAKE:
                {
                    cube_action_t cubeAction = TAKE;
                    pliers->addAction(cubeAction, level);
                    break;
                }

                case PLIERS_RELEASE:
                {
                    cube_action_t cubeAction = RELEASE;
                    pliers->addAction(cubeAction, level);
                    break;
                }

                case PLIERS_GOTO_LEVEL:
                {
                    pliers->setLevel(level);
                    break;
                }

                default:
                    break;
            }

        }

        default:
            break;

    }
}

void handleSonars()
{
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
        CanSender::canSend(sonarFrame);
        // std::vector<uint8_t>::iterator it;
        // for (it = sonar_dist.begin(); it != sonar_dist.end(); it++)
        // {
        //     Serial.print(*it);
        //     Serial.print("|");
        // }
        // Serial.println();
        //sonarArray.disableAll();
        
        //sonarArray.enableAll();
    
        sonarMillis = millis();
    }
}