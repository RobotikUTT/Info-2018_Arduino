#ifndef XBEE_SX868_H
#define XBEE_SX868_H

/** Includes **/
/**************/

#include "Arduino.h"
#include <AnySerial.h>
#include <NeoSWSerial.h>

/** Defines **/
/*************/
#define CHANNEL_0	(uint8_t)0	 
#define CHANNEL_1	(uint8_t)1
#define CHANNEL_2	(uint8_t)2
#define CHANNEL_3	(uint8_t)3
#define CHANNEL_4	(uint8_t)4
#define CHANNEL_5	(uint8_t)5
#define CHANNEL_6	(uint8_t)6
#define CHANNEL_7 	(uint8_t)7

#define PAYLOAD_SIZE	8
#define API_FRAME_SIZE 	100
#define ID_SIZE			2
/** Class Descritpion **/


void setChannel(int); 

class XBee_SX868
{
	private:
		AnySerial* m_serial;
		bool isBusy; //variable qui définit si le XBee est occupé ou non
		bool new_data;
		byte frame_ID[2] = {'I', 'D'}; //initialisation de l'ID de la trame à envoyer
		byte api_frame[API_FRAME_SIZE];  //initialisation de la frame à recevoir XBee. 100 parcequ'on ne connait pas la taille. (à modifier)
		byte received_ID[ID_SIZE];  //initialisation de l'ID reçu (du XBee)
		byte received_payload[PAYLOAD_SIZE]; //initialisation des données recues (du XBee)
		int error_watch;  //variable qui compte les erreurs XBee
		int busyCnt;  //Compte le nombre de fois que le module est Busy d'affilée. Permet de détecter si lemodule a crashé (ça peut arriver...)
		byte frame_number;  //Le numéro de trame est indicatif, il permet de valider l'émission réussie (mode API du XBee...)
		byte frame_header[17] = {0x7E, 0x00, 0x18, 0x10, frame_number, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00}; //Broadcast
		//byte frame_payload[8];
		void openATmode(void); //ouvrir le mode de configuration du XBee
		void closeATmode(void); //fermer le mode de configuration du XBee
		void decodeFrame(void);  //décoder la trame reçue du XBee

	public:
		XBee_SX868(AnySerial* anySerial);
		~XBee_SX868();

		void setChannel(uint8_t mode); //définir le channel utilisé par le XBee
		void reset(void);  //Soft reset le module XBee
		void receive (void); //recoit une trame du module XBee
		void send (byte* frame_payload); //envoie une trame au module XBee
		void checkStatus (void);  //vérifie que le module XBee est toujours up
		byte* getReceivedPayload();
		byte* getReceivedID();
		byte* getAPIFrame();
		void setAddress(int address);
		bool dataAvailable(void);
		
		

};


#endif //XBEE_SX868_H