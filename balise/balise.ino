#include "pins.h"
#include "parameters.h"
#include <PololuA4983.h>

bool direction = 0;
int16_t last_step = 0;
int16_t beginning_object_step = 0;
uint16_t angle = 50; //angle a parcourir en pas du moteur 

bool button_direction = 0;
bool button_direction_memory = 1;
bool button_color = 0;
bool button_color_memory = 1;
bool button_balise_location = 0;
bool button_balise_location_memory = 1;
bool e;//pour boutons

PololuA4983 Stepper(STEPPER_STEP_PIN, STEPPER_DIR_PIN, STEPPER_EN_PIN);

void setup()
{
	Serial.begin(57600);

	pinMode(CAPTEUR_PIN, INPUT);

	//buttons pin :
	//pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(BUTTON_COLOR_PIN, INPUT_PULLUP);
	pinMode(BUTTON_BALISE_LOCATION_PIN, INPUT_PULLUP);

	//leds pin :
	pinMode(LED_LOCATION_RIGHT_PIN, OUTPUT);
	pinMode(LED_LOCATION_CENTER_PIN, OUTPUT);
	pinMode(LED_LOCATION_LEFT_PIN, OUTPUT);
	pinMode(LED_COLOR_GREEN_PIN, OUTPUT);
	pinMode(LED_COLOR_RED_PIN, OUTPUT);

	//test des leds
	digitalWrite(LED_LOCATION_RIGHT_PIN, HIGH);
	digitalWrite(LED_LOCATION_CENTER_PIN, HIGH);
	digitalWrite(LED_LOCATION_LEFT_PIN, HIGH);
	digitalWrite(LED_COLOR_GREEN_PIN, HIGH);
	digitalWrite(LED_COLOR_RED_PIN, LOW);

	Serial.println("Initialisation OK");
}

void loop()
{	
	/*e = digitalRead(BUTTON_PIN);
	if(e == LOW)
	{
		if(button_direction_memory != e)
		{
		    button_direction = !button_direction;
		    Stepper.stop();
		}
	    if(Stepper.getRemainingStep() == 0)
	    {
	        Stepper.moveStep(1, button_direction);
	    }
	}

	else if(Stepper.getRemainingStep() == 0)
	{
		if(direction == 0)
		{
		    Stepper.moveStep(100, 1);
		    direction = 1;
		}
		else
		{
			Stepper.moveStep(100, 0);
		    direction = 0;
		}
	    
	}*/

	if(Stepper.getRemainingStep() == 0)
	{
		if(direction == 0)
		{
		    Stepper.moveStep(angle, 1);
		    direction = 1;
		    Serial.print(" step 1 50 ");
		}
		else
		{
			Stepper.moveStep(angle, 0);
		    direction = 0;
		    Serial.print(" step 0 50 ");
		}
	    
	}

	button_direction_memory = e;

	if(last_step != Stepper.getPositionStep()){
	    if(digitalRead(CAPTEUR_PIN) == HIGH){
	        if(abs(last_step - Stepper.getPositionStep()) <= 1){
	            last_step = Stepper.getPositionStep();
	        }
	        else{
	        	if(last_step < Stepper.getPositionStep()){
	        	    Serial.print("Obstacle de ");
	        		Serial.print(last_step);
	        		Serial.print(" a ");
	        		Serial.println(Stepper.getPositionStep());
	        	}
	        	else{
	        	    Serial.print("Obstacle de ");
	        		Serial.print(Stepper.getPositionStep());
	        		Serial.print(" a ");
	        		Serial.println(last_step);
	        	}
	        	
				last_step = Stepper.getPositionStep();
	        }
	    }
	}

	Stepper.update();

	Serial.println();




}