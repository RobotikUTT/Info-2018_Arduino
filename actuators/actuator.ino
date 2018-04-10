#include "pins.h"
#include <SPI.h>
#include <mcp2515.h>
#include "protocol.h"
#include "PololuA4983.h"
#include "Pliers.h"

// void updateCan (void);

// void canTransmit (void);

PololuA4983 stepper(STEP_PIN,DIR_PIN,EN_PIN);
Pliers pliers(&stepper, PLIERS_LIMIT_SWITCH_PIN);
//MCP2515 mcp2515(SPI_SS_PIN);

struct can_frame canRxMsg;  //strcture de la trame CAN reçue
//struct can_frame canTxMsg; //trame CAN à envoyer



uint32_t orderMillis = millis();
void setup()
{
	Serial.begin(57600);
	Serial.println("Hello World!");
	//pinMode(13, OUTPUT);
  	//SPI.begin();

 //  	canTxMsg.can_id = BBB_CAN_ADDR;
	// canTxMsg.can_dlc = 8;

 //  	mcp2515.reset();
	// mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);  //paramètres les plus rapides testés pour le CAN
	// mcp2515.setNormalMode();
	pliers.reset();
	// stepper.moveStep(400,true);
	// digitalWrite(DIR_PIN, LOW);
	//pliers.setLevel(2);

}

void loop()
{
	// stepper.update();
	// updateCan();

	pliers.update();

}






// void updateCan (){
//   MCP2515::ERROR can_error = mcp2515.readMessage(&canRxMsg);// == MCP2515::ERROR_OK
//   switch(can_error)
//   {
//     case MCP2515::ERROR_OK:
//     	parseAndExecuteOrder(canRxMsg.data);
//       // for (int i = 0; i<canMsgRx.can_dlc+1; i++)
//       // {
//       //     payload[i]=canMsgRx.data[i];
//       // }
//       // affiche = 1;  //on autorise l'affichage
//       break;

//     case MCP2515::ERROR_FAIL:
//       Serial.print("fail\n");
//       break;

//     case MCP2515::ERROR_ALLTXBUSY:
//       Serial.print("busy\n");
//       break;

//     case MCP2515::ERROR_FAILINIT:
//       Serial.print("init fail\n");
//       break;

//     case MCP2515::ERROR_FAILTX:
//       Serial.print("tx fail\n");
//       break;

//     case MCP2515::ERROR_NOMSG:
//       //Serial.print("no msg\n");
//       break;    

//   }
  
// }

