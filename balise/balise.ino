#include "pins.h"
#include "parameters.h"
#include <PololuA4983.h>

void ledUpdate();

void objectDetection(int16_t *min, int16_t *max, bool *new_object);

#define RED 	0
#define GREEN 	1

#define LEFT 	0
#define CENTER 	1
#define RIGHT 	2

#define CHOOSE_MODE 	0
#define CALIBRATION 	1
#define LOCALISATION 	2

uint8_t direction = 0;
int16_t last_step = 0;
int16_t beginning_object_step = 0;
uint16_t angle = 0; //angle a parcourir en pas du moteur 

uint8_t button_direction = 0;
uint8_t button_direction_memory = 1;
uint8_t color = RED;
uint8_t button_color_memory = 1;
uint8_t balise_location = CENTER;
uint8_t button_balise_location_memory = 1;
uint8_t e;//pour boutons

uint8_t baliseState = CHOOSE_MODE;

PololuA4983 Stepper(STEPPER_STEP_PIN, STEPPER_DIR_PIN, STEPPER_EN_PIN, MIN_DELAY);

uint32_t button_delay = millis();

int16_t object_min;
int16_t object_max;
bool new_object = false;
uint8_t object_direction = 0; //empeche la detection d'un objet quand on change de direction
//donc empeche la detection sur le cote si on ne detecte pas la fin de l'obstacle (s'il n'y a pas 2 bornes)

float object_min_mean = 0;
float object_max_mean = 0;
uint8_t object_measure_number = 0;//nombre de mesures moyennees

//var pour calcul coordonnees :
float ro;
float teta;
float delta_teta;
float x;
float y;


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

	Serial.println("Initialisation finished");
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
	if(baliseState == CHOOSE_MODE)			/*###########    Choose Mode    ###########*/
	{
		e = digitalRead(BUTTON_COLOR_PIN);
		if((e == LOW) && (button_color_memory == HIGH))
		{
			color = !color;
		}
		button_color_memory = e;

		e = digitalRead(BUTTON_BALISE_LOCATION_PIN);
		if((e == LOW) && (button_balise_location_memory == HIGH))
		{
			button_delay = millis();
		}
		if((e == HIGH) && (button_balise_location_memory == LOW))
		{
		    balise_location++;
		    if(balise_location > 2)
		    {
		        balise_location = 0;
		    }

		}
		button_balise_location_memory = e;
		if((millis() - button_delay > BUTTON_DELAY) && (digitalRead(BUTTON_BALISE_LOCATION_PIN) == LOW))
		{
			baliseState = CALIBRATION;
			Serial.println("Choose mode finished");
		}
	}
	else if(baliseState == CALIBRATION)		/*###########    Calibration    ###########*/
	{
	    /*switch (color) {
	        case RED:
	          switch (balise_location) {
	              case LEFT:
	                
	                break;
	              case CENTER:
	                
	                break;
	              case RIGHT:
	                
	                break;
	          }
	          break;
	        case GREEN:
	          switch (balise_location) {
	              case LEFT:
	                
	                break;
	              case CENTER:
	                
	                break;
	              case RIGHT:
	                
	                break;
	          }
	          break;
	    }*/

	    if(Stepper.getRemainingStep() == 0)
		{
			if(Stepper.m_position_step == 0)
			{
				if(direction == 0)
				{
					if(new_object == false)//si on a pas trouve de balise on augmente la plage du balayage
					{
						angle = angle + 10;
						object_measure_number = 0;
					}
					else
					{
						new_object = false;
						object_measure_number++;
						object_min_mean = (object_measure_number - 1)*object_min_mean/object_measure_number + object_min/object_measure_number;
						object_max_mean = (object_measure_number - 1)*object_max_mean/object_measure_number + object_max/object_measure_number;
						if(object_measure_number >= NUMBER_OBJECT_2_MEASURE)
						{
							if(balise_location == CENTER)
							{
								angle = ANGLE*2;
							}
							else
							{
								angle = ANGLE;
							}

							if(balise_location == LEFT)
							{
								Stepper.moveStep((object_min_mean+object_max_mean)/2, 0);
							}
							else
							{
								Stepper.moveStep((object_min_mean+object_max_mean)/2, 1);
							}

							do
							{
								Stepper.update();
								//Serial.println(Stepper.getRemainingStep());
								delay(5);
							}while(Stepper.getRemainingStep() != 0);
							Stepper.m_position_step = 0;
							baliseState = LOCALISATION;
							Serial.println("Calibration finished");
						}
						Serial.print("Obstacle de ");
						Serial.print(object_min_mean);
						Serial.print(" a ");
						Serial.println(object_max_mean);
					}
				}
				//if(baliseState != LOCALISATION)
				//{
					Stepper.moveStep(angle, direction);
				//}
			}
			if(abs(Stepper.m_position_step) == angle)
			{
				direction = !direction;
				Stepper.moveStep(angle, direction);
			}		    
		}
		objectDetection(&object_min, &object_max, &new_object);
		/*if(new_object == true)
		{
			Serial.print("Obstacle de ");
			Serial.print(object_min);
			Serial.print(" a ");
			Serial.println(object_max);
			//new_object = false;
		}*/
	}
	else if(baliseState == LOCALISATION)	/*###########    Localisation    ###########*/
	{
		if(Stepper.getRemainingStep() == 0)
		{
			if(direction == 0)
			{
			    Stepper.moveStep(angle, 1);
			    direction = 1;
			    //Serial.println(Stepper.m_position_step);
			}
			else
			{
				Stepper.moveStep(angle, 0);
			    direction = 0;
			    //Serial.println(Stepper.m_position_step);
			}
		    
		}

		button_direction_memory = e;

		objectDetection(&object_min, &object_max, &new_object);

		if(new_object == true)
		{
			teta = (object_min + object_max)*PI_100/2; //teta en radian
			delta_teta = object_max - object_min; //attention delta_teta est en pas
			ro = LIN_REG_A*delta_teta+LIN_REG_B;
			x = (ro*cos(teta)+94)*P;
			y = (ro*sin(teta)-28)*P*1.6;
			Serial.print("Obstacle de ");
    		Serial.print(object_min);
    		Serial.print(" a ");
    		Serial.print(object_max);
    		Serial.print("  |  coordonnees : x = ");
    		Serial.print(x);
    		Serial.print(" et y = ");
    		Serial.println(y);
    		new_object = false;
		}

		/*if(last_step != Stepper.m_position_step){
		    if(digitalRead(CAPTEUR_PIN) == HIGH){
		        if(abs(last_step - Stepper.m_position_step) <= 1){
		            last_step = Stepper.m_position_step;
		        }
		        else{
		        	if(last_step < Stepper.m_position_step){
		        	    Serial.print("Obstacle de ");
		        		Serial.print(last_step);
		        		Serial.print(" a ");
		        		Serial.println(Stepper.m_position_step);
		        	}
		        	else{
		        	    Serial.print("Obstacle de ");
		        		Serial.print(Stepper.m_position_step);
		        		Serial.print(" a ");
		        		Serial.println(last_step);
		        	}
		        	
					last_step = Stepper.m_position_step;
		        }
		    }
		}*/
	}

	Stepper.update();

	ledUpdate();

	//Serial.print(millis() - button_delay);

	//Serial.println(digitalRead(BUTTON_BALISE_LOCATION_PIN));
	//delay(3);




}

void ledUpdate()
{
	switch (color) {
	    case RED:
	      digitalWrite(LED_COLOR_RED_PIN, HIGH);
	      digitalWrite(LED_COLOR_GREEN_PIN, LOW);
	      break;
	    case GREEN:
	      digitalWrite(LED_COLOR_RED_PIN, LOW);
	      digitalWrite(LED_COLOR_GREEN_PIN, HIGH);
	      break;
	}
	switch (balise_location) {
	    case LEFT:
	      digitalWrite(LED_LOCATION_LEFT_PIN, HIGH);
	      digitalWrite(LED_LOCATION_CENTER_PIN, LOW);
	      digitalWrite(LED_LOCATION_RIGHT_PIN, LOW);
	      break;
	    case CENTER:
	      digitalWrite(LED_LOCATION_LEFT_PIN, LOW);
	      digitalWrite(LED_LOCATION_CENTER_PIN, HIGH);
	      digitalWrite(LED_LOCATION_RIGHT_PIN, LOW);
	      break;
	    case RIGHT:
	      digitalWrite(LED_LOCATION_LEFT_PIN, LOW);
	      digitalWrite(LED_LOCATION_CENTER_PIN, LOW);
	      digitalWrite(LED_LOCATION_RIGHT_PIN, HIGH);
	      break;
	}
}

void objectDetection(int16_t *min, int16_t *max, bool *new_object)
{
	if(last_step != Stepper.m_position_step){
		    if(digitalRead(CAPTEUR_PIN) == HIGH){
		        if((abs(last_step - Stepper.m_position_step) <= 1) || (object_direction != direction)){
		            last_step = Stepper.m_position_step;
		            object_direction = direction;
		        }
		        else{
		        	/*if(last_step < Stepper.m_position_step){
		        		*min = last_step;
		        		*max = Stepper.m_position_step;
		        	}
		        	else{
		        		*min = Stepper.m_position_step;
		        	    *max = last_step;
		        	}*/
		        	*min = min(last_step, Stepper.m_position_step);
		        	*max = max(last_step, Stepper.m_position_step);
		        	*new_object = true;
					last_step = Stepper.m_position_step;
		        }
		    }
		}
}