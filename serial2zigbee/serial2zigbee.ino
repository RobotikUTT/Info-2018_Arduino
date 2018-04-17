#include <SPI.h>
#include <SoftwareSerial.h>
#include <NeoSWSerial.h>
#include <AnySerial.h>
#include "protocol.h"
#include "mcp2515.h"
#include "XBee_SX868.h"
#include "SwitchManager.h"
#define MAX_TAILLE  30
#define RX_PIN      11
#define TX_PIN      12

NeoSWSerial softSerial(RX_PIN,TX_PIN);
AnySerial XBeeSerial(&softSerial);

XBee_SX868 XBee(&XBeeSerial);
SwitchManager switchManager;
// struct can_frame canMsgRx; //trame CAN à recevoir
// struct can_frame canMsgTx; //trame CAN à envoyer
//MCP2515 mcp2515(10);

// void canTransmit (void);
// void getCan (void);
void lireMessage();

void checkSerial (void);

void setup()
{
	Serial.begin(57600);

	softSerial.begin(38400);
	delay(50);
  Serial.println("begin");
	int address = switchManager.scanAddress();
	
	int channel = switchManager.scanChannel();
  delay(50);
	XBee.setChannel(channel); //char conversion
	
	XBee.setAddress(address);
	//pinMode(13,OUTPUT);
	//digitalWrite(13,HIGH);

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
	// mcp2515.setBitrate(500KBPS, MCP_16MHZ);  //paramètres les plus rapides testés pour le CAN
	// mcp2515.setNormalMode();
  Serial.println("Noublie pas d'envoyer un caractere de fin de ligne ;)");
	delay(200);
	
}

void loop()
{
	XBee.receive();
	lireMessage();
	
	if ( XBee.dataAvailable() )
	{
		byte* data = XBee.getReceivedPayload();
		byte* id_array = XBee.getReceivedID();
		int id = id_array[0] << 8 | id_array[1];
		
    decodeFrame(data);

	}
	XBee.checkStatus();
	// canTransmit();
	// getCan();

}

void checkSerial (void){
  if(Serial.available()){
  	//Serial.println("available");
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

void lireMessage() {
    if (Serial.available()) {
        // Buffer d'entrée
        
        char buffer_in[MAX_TAILLE + 1];
        //Serial.println("check serial");
        byte size = Serial.readBytesUntil('\n',buffer_in, MAX_TAILLE);
        //Serial.println("serial read");
        // On ajoute un octet null pour marquer la fin de la String
        buffer_in[size] = 0;

        // On lis chaque argument, séparés par des `:`
        char* args = strtok(buffer_in, ":");

        char commande = buffer_in[0];
        args = strtok(0, ":");

        uint8_t message[MSG_SIZE];
        
        switch (commande) {
            case HANDSHAKE:
          {
              // Ack that stm has started
              Serial.println("HANDSHAKE");
              encodeFrame(message, CAN_HANDSHAKE);
              XBee.send(message);
              //CanSender::canSend(WHOAMI,16);
              //Serial.print("HANDSHAKE\n");
              // CanSender::canSend(SERIAL_INFO, "%d;", order_id);
              // CanSender::canSend(SERIAL_DEBUG, "Arduino %s has started (%d)", ARDUINO_ID, order_id);
              
              break;
          }
  
          // case WHOAMI:
          // {
          //     break;
          // }
  
          //case SET_MODE:
          //{
          //    break;
          //}
  
          case SPD:
          {
              int l, a, t;
          
              l = atoi(args);
              args = strtok(0, ":"); 
    
              a =     atoi(args);
              args = strtok(0, ":"); 
              
              t =     atoi(args);
              args = strtok(0, ":"); 
              Serial.print(l);
              Serial.print("\n");
              Serial.print(a);
              Serial.print("\n");
              Serial.print(t);
              Serial.print("\n");
              
              encodeFrame(message, CAN_SPD,l,a,t);
              XBee.send(message);
              
              break;
          }
  
          case GET_CODER:
          {
              encodeFrame(message, GET_CODER);
              XBee.send(message);
              break;
          }
  
          
              case HALT:
              {
                  Serial.print("STOP");
                  Serial.print("\n");
                  encodeFrame(message, GET_CODER,CAN_STOP);
                  XBee.send(message);
                  break;
              }
              case START:
              {   
                  Serial.print("START");
                  Serial.print("\n");
                  
                  encodeFrame(message, CAN_MANAGEMENT, CAN_START);
                  XBee.send(message);
                  break;
              }
              case PAUSE:
              {
                  Serial.print("PAUSE");
                  Serial.print("\n");
                  encodeFrame(message, CAN_MANAGEMENT, CAN_PAUSE);
                  XBee.send(message);
                  break;
              }
              case RESUME:
              {
                  Serial.print("RESUME");
                  Serial.print("\n");
                  encodeFrame(message, CAN_MANAGEMENT,CAN_RESUME);
                  XBee.send(message);
                  break;
              }
              case RESET_ID:
              {
                  Serial.print("RESET_ID");
                  Serial.print("\n");
                  encodeFrame(message, CAN_MANAGEMENT,CAN_STOP);
                  XBee.send(message);
                  break;
              }
              case SETEMERGENCYSTOP:
              {
                  Serial.print("EMGSTOP");
                  Serial.print("\n");
                  encodeFrame(message, CAN_MANAGEMENT,CAN_SETEMERGENCYSTOP);
                  XBee.send(message);
                  break;
              }
              case KILLG:
              {
                  Serial.print("NEXT GOAL");
                  Serial.print("\n");
                  encodeFrame(message, CAN_MANAGEMENT,CAN_NEXT_ORDER);
                  XBee.send(message);
                  break;
              }
              case CLEANG:
              {
                  Serial.print("RESET GOALS");
                  Serial.print("\n");
                  encodeFrame(message, CAN_MANAGEMENT,CAN_RESET_ORDERS);
                  XBee.send(message);
                  break;
              }
              
  
          
  
          case GOTOA:
          {
              Serial.print("GOTOA");
              Serial.print("\n");
              int x, y, a_int, direction;
              direction = 0;

              x =     atoi(args);
              args = strtok(0, ":"); 
              y =     atoi(args);
              args = strtok(0, ":"); 
              a_int =     atoi(args);
              args = strtok(0, ":"); 
              direction =     atoi(args);
              args = strtok(0, ":"); 
              encodeFrame(message, CAN_GOTOA,x,y,a_int,direction);
              XBee.send(message);
  
              
  
              Serial.print(x);
              Serial.print("\n");
              Serial.print(y);
              Serial.print("\n");
              Serial.print(a_int);
              Serial.print("\n");
              Serial.print(direction);
              Serial.print("\n");
  
              
              break;
          }
  
          case GOTO:
          {
              Serial.print("GOTO");
              Serial.print("\n");
              int16_t x, y;
              int8_t direction;
  
              x =     atoi(args);
              args = strtok(0, ":"); 
              y =     atoi(args);
              args = strtok(0, ":");  
              direction =     atoi(args);
              args = strtok(0, ":"); 
              encodeFrame(message, CAN_GOTO,x,y,direction);
              XBee.send(message);
  
              Serial.print(x);
              Serial.print("\n");
              Serial.print(y);
              Serial.print("\n");
  
              
              break;
          }
  
          case ROT:
          {
              Serial.print("ROT");
              Serial.print("\n");
              int16_t a_int;
  
               
              a_int =     atoi(args);
              
              encodeFrame(message, CAN_ROT,a_int);
              XBee.send(message);
  
              Serial.print(a_int);
              Serial.print("\n");
  
              
              break;
          }
  
          case ROTNOMODULO:
          {
              Serial.print("ROT NO MODULO");
              Serial.print("\n");
              long a_int;
              
              
              a_int =     atoi(args);
              args = strtok(0, ":"); 
              
              
              encodeFrame(message, CAN_ROTNOMODULO,a_int);
              XBee.send(message);
              Serial.print(a_int);
              Serial.print("\n");
              
  
              break;
          }
  
          case PIDLEFT:
          case PIDRIGHT:
          case PIDALL:
          {
              long p_int, i_int, d_int;
              
              p_int =     atoi(args);
              args = strtok(0, ":"); 
              i_int =     atoi(args);
              args = strtok(0, ":"); 
              d_int =     atoi(args);
              args = strtok(0, ":"); 
              
              
              
  
              Serial.print(p_int);
              Serial.print("\n");
              Serial.print(i_int);
              Serial.print("\n");
              Serial.print(d_int);
              Serial.print("\n");
              
              
  
              
              if (commande == PIDLEFT)
              {
                  Serial.print("PID LEFT");
                  Serial.print("\n");
                  encodeFrame(message, CAN_PIDLEFT,p_int,i_int,d_int);
              }
              else if (commande == PIDRIGHT)
              {
                  Serial.print("PID RIGHT");
                  Serial.print("\n");
                  encodeFrame(message, CAN_PIDRIGHT,p_int,i_int,d_int);
              }
              else
              {
                Serial.print("PID ALL\n");
                encodeFrame(message, CAN_PIDALL,p_int,i_int,d_int);
              }
              Serial.println("about to send");
              XBee.send(message);
              Serial.println("sent");
              break;
          }
  
          case PWM:
          {
              Serial.print("PWM");
              Serial.print("\n");
              int16_t l, r;
              uint16_t t;
  
              l =     atoi(args);
              args = strtok(0, ":"); 
              r =     atoi(args);
              args = strtok(0, ":"); 
              t =     atoi(args);
              args = strtok(0, ":"); 
              
              encodeFrame(message, CAN_PWM,l,r,t);
              XBee.send(message);
  
              Serial.print(l);
              Serial.print("\n");
              Serial.print(r);
              Serial.print("\n");
              Serial.print(t);
              Serial.print("\n");
  
              
              break;
          }
  
          case SET_POS:
          {
              Serial.print("SET POS");
              Serial.print("\n");
              int16_t x, y, a_int;

              x =     atoi(args);
              args = strtok(0, ":"); 
              y =     atoi(args);
              args = strtok(0, ":"); 
              a_int =     atoi(args);
              args = strtok(0, ":"); 
              
              encodeFrame(message, CAN_SET_POS,x,y,a_int);
              XBee.send(message);
              
  
              Serial.print(x);
              Serial.print("\n");
              Serial.print(y);
              Serial.print("\n");
              Serial.print(a_int);
              Serial.print("\n");
  
              
  
  
  
              
              break;
          }
  
          case ACCMAX:
          {
              Serial.print("SET PARAM");
              Serial.print("\n");
              int16_t r_int, s,a;
              
              s =     atoi(args);
              args = strtok(0, ":"); 
              r_int =     atoi(args);
              args = strtok(0, ":"); 
              a =     atoi(args);
              args = strtok(0, ":"); 
              
              encodeFrame(message, CAN_SET_PARAM,s,r_int,a);
              XBee.send(message);
  
              Serial.print(s);
              Serial.print("\n");
              Serial.print(r_int);
              Serial.print("\n");
              Serial.print(a);
              Serial.print("\n");
  
              
              break;
          }
            
        }
        
    }
    
}
