

/** Includes **/
/**************/
#include "SwitchManager.h"

/** Constructor **/
/*****************/

SwitchManager::SwitchManager()
{

}
/*** Destructor **/
/*****************/

SwitchManager::~SwitchManager()
{

}


/** Public Methods **/
/********************/

int SwitchManager::scanAddress(){  //Lecture des switchs d'adresse
  int value = 0;
  value = value + !digitalRead(ADDRESS_BIT_1);
  value = value + !digitalRead(ADDRESS_BIT_2) * 2;
  value = value + !digitalRead(ADDRESS_BIT_3) * 4;
  value = value + !digitalRead(ADDRESS_BIT_4) * 8;
  value = value + !digitalRead(ADDRESS_BIT_5) * 16;
  value = value + !digitalRead(ADDRESS_BIT_6) * 32;
  value = value + !digitalRead(ADDRESS_BIT_7) * 64;
  value = value + !digitalRead(ADDRESS_BIT_8) * 128;
  value = value + !digitalRead(ADDRESS_BIT_9) * 256;
  value = value + !digitalRead(ADDRESS_BIT_10) * 512;
  value = value + !digitalRead(ADDRESS_BIT_11) * 1024;
  return value;
}

int SwitchManager::scanChannel(void){  //Lecture des switchs de channel radio
  int value = 0;
  value = value + !digitalRead(CHANNEL_BIT_1);
  value = value + !digitalRead(CHANNEL_BIT_2) * 2;
  value = value + !digitalRead(CHANNEL_BIT_3) * 4;
  return value;
}


/** Private Methods **/
/*********************/