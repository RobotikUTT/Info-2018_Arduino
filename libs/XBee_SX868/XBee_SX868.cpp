#include "XBee_SX868.h"

/** Includes **/
/**************/


/** Constructor **/
/*****************/

XBee_SX868::XBee_SX868(AnySerial* anySerial)
{
  m_serial = anySerial;
	// if (!m_serial)
	// 	m_serial->begin(57600);
	// delay(50);
	isBusy = true;
	error_watch = 0;
  // frame_ID[0] = 'I';
  // frame_ID[1] = 'D';
  frame_number = 0;
  // frame_header = {0x7E, 0x00, 0x18, 0x10, 
  //                     frame_number, 0x00, 0x00, 0x00, 
  //                   0x00, 0x00, 0x00, 0xFF, 
  //                   0xFF, 0xFF, 0xFE, 0x00, 
  //                   0x00};
  busyCnt = 0;
  new_data = false;

}

		
/*** Destructor **/
/*****************/
XBee_SX868::~XBee_SX868()
{
  delete m_serial;
}

/** Public Methods **/
/********************/



void XBee_SX868::setChannel(uint8_t mode){  //Paramétrage du channel radio
  openATmode(); //Ouverture du mode commande
  m_serial->write('A');  //Commande permettant de modifier le channel d'émission
  m_serial->write('T');
  m_serial->write('C');
  m_serial->write('M');
    
  switch (mode){   //Sélection des différents channels (3 channels de 200kHz adjacents) en fonction du mode
    case CHANNEL_0:
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('7');
    break;
    
    case CHANNEL_1:
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('3');
    m_serial->write('8');

    break;
    
    case CHANNEL_2:
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('5');
    m_serial->write('8');
    m_serial->write('0');
    break;
    
    case CHANNEL_3:
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('3');
    m_serial->write('8');
    m_serial->write('0');
    m_serial->write('0');
    break;
    
    case CHANNEL_4:
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('1');
    m_serial->write('C');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    break;
    
    case CHANNEL_5:
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('E');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    break;
    
    case CHANNEL_6:
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('7');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    break;
    
    case CHANNEL_7:
    m_serial->write('0');
    m_serial->write('6');
    m_serial->write('8');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    m_serial->write('0');
    break;
    
    default:
    break;
  }
  m_serial->write('\r');
  delay(500);
  m_serial->write('A');  //Commande permettant de charger les changement en EEPROM
  m_serial->write('T');
  m_serial->write('W');
  m_serial->write('R');
  m_serial->write('\r');
  delay(500);
  m_serial->write('A');  //Application des changements
  m_serial->write('T');
  m_serial->write('A');
  m_serial->write('C');
  m_serial->write('\r');
  delay(500);
  closeATmode();  //Quitter le mode de commande AT
}

void XBee_SX868::reset(void){
  openATmode();
  while(m_serial->available()){m_serial->read();} //Le module renvoie "OK" à l'ouvertue
  while(!m_serial->available()){//Commande déclenchant le reset
    m_serial->write('A');
    m_serial->write('T');
    m_serial->write('F');
    m_serial->write('R');
    m_serial->write('\r');
    delay(200);
  }
}

void XBee_SX868::receive (void){
  if(m_serial->available()){ //Si le XBee a quelque chose à nous dire

    if(m_serial->read()==0x7E){  //Delimiter de début de trame API
      new_data = true;
      api_frame[0]=0x7E;
      delay(1);
      int i = 1;
      int x = 26;
      while(i<x){ //On vient lire les autres octets de la trame API. X est fixé ici, mais il sera remplacé par l'octet qui définit la longueur de la trame.
        api_frame[i]=m_serial->read();
        i++;
        delay(1);
      }
    }
    if(api_frame[3]==0x8B){ //frame type 0x8B : transmit status - équivalent ACK. Permet de ne pas spammer le port série du XBee.
      if(api_frame[8]==0x00){ //transmit status 0x00 : success
        for(int j=0; j<=api_frame[2]+4; j++){ //api_frame[2] contient la longueur de la trame, en exlucant le checksum et les 3 premiers octets
          api_frame[j]=0; //on efface la trame API une fois qu'on a fini
        }
        for(int j=0; j<8; j++){
          received_payload[j]=0;  //On efface les données reçues (si on est pas en mode DEMO, auquel cas on garde toujours la même payload).
        }
      }
      else{ //Si le transmit status n'est pas un succès
        error_watch++;
        for(int j=0; j<=api_frame[2]+4; j++){
          api_frame[j]=0; //On nettoie la trame API.
        }
      }
    }
    else if (api_frame[3]==0x90){ //0x90 signale une réception de données par le XBee
      decodeFrame(); //on décoe la trame reçue
    }
    else{
      for(int j=0; j<=30; j++){
          api_frame[j]=0; //On nettoie la trame API (garbage)
      }
    }
    isBusy = false; //On est sûr que le module est libre maintenant
  }
}

void XBee_SX868::send(byte *frame_payload)
{
  if(((!isBusy) || (busyCnt==0xFFFF)) && (frame_payload[0]!=0) && !m_serial->available()){ //Si le module est dispo et qu'on a une payload à envoyer
    //Note : la valeur de BusyCount mise ici permet de forcer une communication avec le module une fois de temps en temps même si il est censé être Busy.
    //Cela permet depasser outre une éventuelle erreur de flag Busy, sans avoir à reset.
    
    byte checksum = 0;  //Initialisation du checksum
    if(frame_number == 0) {frame_number++;} //Pour ne pas avoir de nombre négatif en frame_number (rappel : valeur à titre indicatif)
    for (int i =0; i<3; i++){
      m_serial->write(frame_header[i]);  //On envoie les trois premiers octets de la trame API, le checksum ne s'applique pas à ceux-ci
    }
    for (int i =3; i<17; i++){
      checksum += frame_header[i];
      m_serial->write(frame_header[i]);  //On envoie le reste du header (qui est toujours le même car on envoie toujours 8 bits de la même manière) tout en gérant le checksum
    }
    for (int i=0; i<2; i++){
      checksum += frame_ID[i];
      m_serial->write(frame_ID[i]);  //On envoie l'ID tout en gérant le checksum
    }
    for (int i=0; i<8; i++){
      checksum += frame_payload[i];
      m_serial->write(frame_payload[i]); //On envoie la donnée tout en gérant le checksum
    }
    checksum = 0xFF - checksum; //Finalisation du checksum (c'est la formule donnée dans la datasheet)
    m_serial->write(checksum); //On envoie le checksum
  isBusy = true;  //Le module est désormais occupé jusqu'à ce qu'il déclare qu'il a finit d'envoyer la trame (transmit status packet).
  busyCnt = 0;  //Le busycnt est remis à 0
  frame_number++; //Incrémentation du frame_number
    delay(50);
    while(m_serial->available()){m_serial->read();} //On vide le buffer m_serial, pour ne pas avoir accumulé de latence pendant cette phase d'envoi.
  }
  else if (isBusy==true){ //Si le module n'est pas prêt à envoyer de trame
    busyCnt ++;
    if(busyCnt==0xFFFE){
      error_watch++;  //Si le module est Busy pendant trop longtemps, on ajoute une erreur au compteur
      m_serial->println("XBee busy...");
      isBusy = false; //On libère isBusy pour tenter de renvoyer de la donnée, au cas où le module ait planté où une mauvaise manipulation du flag Busy.
    }
  }
}

void XBee_SX868::checkStatus (void){
  if(error_watch>10){ //si on a accumulé plus de 10 erreurs, reset.
    //Un crash peut être dû au buffer XBee qui sature (côté ZigBee, pas m_serial)
    //Le module peut ne pas répondre si il n'est pas correctement sorti du mode de commande.
    
    reset();
    delay(1000);
    error_watch = 0;  //réinitialisation des erreurs
    while(m_serial->available()){m_serial->read();} //On vide les buffers m_serial qui ont pu se remplir pendant le reset
    isBusy = false;//on libère le flag busy, au cas où il ait été mis à 0 avant.
  }
}

byte* XBee_SX868::getReceivedPayload()
{
  new_data = false;
  return received_payload;
}

byte* XBee_SX868::getReceivedID()
{
  return received_ID;
}

byte* XBee_SX868::getAPIFrame()
{
  return api_frame;
}

void XBee_SX868::setAddress(int address)
{
  frame_ID[0] = address >> 8;
  frame_ID[1] = address;
}

bool XBee_SX868::dataAvailable()
{
  return new_data;
}

/** Private Methods **/
/*********************/
void XBee_SX868::openATmode(void){  //Ouverture du mode AT
  while(m_serial->available()){m_serial->read();} //On vide le buffer m_serial
  while(!m_serial->available()){ //Tant que le module ne répond pas OK, on continue à demander le mode AT
    m_serial->write('+');  //Commande pour déclencher le mode AT
    m_serial->write('+');
    m_serial->write('+');
    delay(1200);
  }
}

void XBee_SX868::closeATmode(void){ //Fermeture du mode AT
  while(m_serial->available()){m_serial->read();}
  while(!m_serial->available()){
    m_serial->write('A');
    m_serial->write('T');
    m_serial->write('C');
    m_serial->write('N');
    m_serial->write('\r');
    delay(200);
  }
}

void XBee_SX868::decodeFrame(void){
  for(int i=0; i<2; i++){
    received_ID[i] = api_frame[i+15];
  }
  for(int i=0; i<8; i++){
    received_payload[i] = api_frame[i+17];
  }
  for(int j=0; j<=api_frame[2]+4; j++){
    api_frame[j]=0;
  }
}