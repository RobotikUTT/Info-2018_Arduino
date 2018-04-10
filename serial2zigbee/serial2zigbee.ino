#include <SPI.h>
#include <SoftwareSerial.h>
#include "mcp2515.h"
#include "XBee_SX868.h"
#include "SwitchManager.h"


SoftwareSerial softSerial(11,12);
AnySerial XBeeSerial(&softSerial);
XBee_SX868 XBee(&XBeeSerial);
SwitchManager switchManager;
// struct can_frame canMsgRx; //trame CAN à recevoir
// struct can_frame canMsgTx; //trame CAN à envoyer
//MCP2515 mcp2515(10);

// void canTransmit (void);
// void getCan (void);

void checkSerial (void);

void setup()
{
	Serial.begin(57600);
	delay(50);
	int address = switchManager.scanAddress();
	int channel = switchManager.scanChannel();
	XBee.setChannel(channel); //char conversion
	XBee.setAddress(address);

	//On génère la structure de la trame CAN
	// canMsgTx.can_id  = 0x62;  //L'adresse (de destination) est mise à 0x62 car l'arduino qui va recevoir la trame est bloquée à cette adresse (elle n'a pas de switchs)
	// canMsgTx.can_dlc = 8; //payload toujours 8
	// canMsgTx.data[0] = 0x00;  
	// canMsgTx.data[1] = 0x00;
	// canMsgTx.data[2] = 0x00;
	// canMsgTx.data[3] = 0x00;
	// canMsgTx.data[4] = 0x00;
	// canMsgTx.data[5] = 0x00;
	// canMsgTx.data[6] = 0x00;
	// canMsgTx.data[7] = 0x00;

	// SPI.begin();
	// mcp2515.reset();
	// mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);  //paramètres les plus rapides testés pour le CAN
	// mcp2515.setNormalMode();
	delay(200);
}

void loop()
{
	checkSerial();
	XBee.receive();
	XBee.checkStatus();
	if ( XBee.dataAvailable() )
	{
		byte* data = XBee.getReceivedPayload();
		byte* id_array = XBee.getReceivedID();
		int id = id_array[0] << 8 | id_array[1];
		for ( uint8_t i = 0 ; i < 8 ; i++)
		{
			Serial.print(data[i]);
			Serial.print("|");
		}
		Serial.print("|");
		Serial.println(id);

	}
	// canTransmit();
	// getCan();

}

void checkSerial (void){
  if(Serial.available()){
    int i = 0;
    byte payload[8];
    while(Serial.available()){
      payload[i]=Serial.read();
      Serial.println(payload[i]);
      //Serial.print(frame_payload[i]);
      delay(1);//nécessaire sinon on lit plus vite qu'on ne reçoit
      i++;
      if(i>=8){break;}//On ne travaille qu'avec des trames de 8 octets
    }
    //Serial.println(i);
   // Serial.println(payload[0]);
    XBee.send(payload);
  }
}

// void canTransmit (void){  //Transmission CAN (envoi seulement)
// 	byte* received_payload = XBee.getReceivedPayload();
//   if(received_payload[0]!=0){ //Si on a des données à envoyer
//     for(int i=0; i<8; i++){
//       canMsgTx.data[i] = received_payload[i]; //On associe la data à la structure CAN
//     }
//     mcp2515.sendMessage(&canMsgTx); //On envoie le message au mcp2515
//     delay(10);
//     for(int i=0; i<8; i++){
//       received_payload[i]=0;  //On remet la payload à 0 pour ne pas la renvoyer
//     }
//   }
// }

// void getCan (void){
//   if ((mcp2515.readMessage(&canMsgRx) == MCP2515::ERROR_OK)) { //Si le CAN est disponible
//     for (int i = 0; i<canMsgRx.can_dlc+1; i++)
//     {
//         XBee.send(canMsgRx.data);
//     }
//     //affiche = 1;  //on autorise l'affichage
//     //Serial.println("ERROR_OK");
//   }
//   else if (mcp2515.readMessage(&canMsgRx) == MCP2515::ERROR_FAIL) {
//     //Serial.println("received stuff but failed");
//   }
// }

