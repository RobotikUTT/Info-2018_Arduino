#ifndef SWITCH_MANAGER_H
#define SWITCH_MANAGER_H

/** Includes **/
/**************/

#include "Arduino.h"

/** Defines **/
/*************/

#define ADDRESS_BIT_11	2 //MSB
#define ADDRESS_BIT_10	3
#define ADDRESS_BIT_9	4
#define ADDRESS_BIT_8	5
#define ADDRESS_BIT_7	6
#define ADDRESS_BIT_6	7
#define ADDRESS_BIT_5	8
#define ADDRESS_BIT_4	9
#define ADDRESS_BIT_3	A0
#define ADDRESS_BIT_2	A1
#define ADDRESS_BIT_1	A2


#define CHANNEL_BIT_1 	A5
#define CHANNEL_BIT_2 	A4
#define CHANNEL_BIT_3 	A3 //MSB

/** Class Descritpion **/

class SwitchManager
{
	private:


	public:
		SwitchManager();
		~SwitchManager();
		int scanAddress(void); //scan du switch d'adresse
		int scanChannel(void); //scan du switch de channel (radio)
		

};


#endif //SWITCH_MANAGER_H