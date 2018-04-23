#include <SPI.h>
#include <SoftwareSerial.h>
#include <NeoSWSerial.h>
#include <AnySerial.h>
#include "protocol.h"
#include <mcp2515.h>
#include "XBee_SX868.h"
#include "SwitchManager.h"
#define MAX_TAILLE  30
#define RX_PIN      11
#define TX_PIN      12

NeoSWSerial softSerial(RX_PIN,TX_PIN);
AnySerial XBeeSerial(&softSerial);

XBee_SX868 XBee(&XBeeSerial);
SwitchManager switchManager;

void lireMessage();
void decodeFrame(uint8_t* message);
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
	XBee.setChannel(channel);
	
	XBee.setAddress(address);
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

}

void checkSerial (void){
  if(Serial.available()){
    int i = 0;
    byte payload[8];
    while(Serial.available()){
      payload[i]=Serial.read();
      Serial.println(payload[i]);
      delay(1);//nécessaire sinon on lit plus vite qu'on ne reçoit
      i++;
      if(i>=8){break;}//On ne travaille qu'avec des trames de 8 octets
    }
    
    XBee.send(payload);
  }
}

void lireMessage() {
    if (Serial.available()) {
        // Buffer d'entrée
        
        char buffer_in[MAX_TAILLE + 1];
        byte size = Serial.readBytesUntil('\n',buffer_in, MAX_TAILLE);
        // On ajoute un octet null pour marquer la fin de la String
        buffer_in[size] = 0;

        // On lis chaque argument, séparés par des `:`
        char* args = strtok(buffer_in, ":");

        char commande = buffer_in[0];
        args = strtok(0, ":");

        uint8_t message[MSG_SIZE];
        
        switch (commande) {
            case SERIAL_HANDSHAKE:
          {
              // Ack that stm has started
              Serial.println("HANDSHAKE");
              encodeFrame(message, HANDSHAKE);
              XBee.send(message);
              
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
  
          case SERIAL_SPD:
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
              
              encodeFrame(message, SPD,l,a,t);
              XBee.send(message);
              
              break;
          }
  
          case SERIAL_GET_CODER:
          {
              encodeFrame(message, GET_CODER);
              XBee.send(message);
              break;
          }
  
          
          case SERIAL_HALT:
          {
              Serial.print("STOP");
              Serial.print("\n");
              encodeFrame(message, MANAGEMENT,STOP);
              XBee.send(message);
              break;
          }
          case SERIAL_START:
          {   
              Serial.print("START");
              Serial.print("\n");
              
              encodeFrame(message, MANAGEMENT, START);
              XBee.send(message);
              break;
          }
          case SERIAL_PAUSE:
          {
              Serial.print("PAUSE");
              Serial.print("\n");
              encodeFrame(message, MANAGEMENT, PAUSE);
              XBee.send(message);
              break;
          }
          case SERIAL_RESUME:
          {
              Serial.print("RESUME");
              Serial.print("\n");
              encodeFrame(message, MANAGEMENT,RESUME);
              XBee.send(message);
              break;
          }
          case SERIAL_RESET_ID:
          {
              Serial.print("RESET_ID");
              Serial.print("\n");
              encodeFrame(message, MANAGEMENT,STOP);
              XBee.send(message);
              break;
          }
          case SERIAL_SETEMERGENCYSTOP:
          {
              Serial.print("EMGSTOP");
              Serial.print("\n");
              encodeFrame(message, MANAGEMENT,SETEMERGENCYSTOP);
              XBee.send(message);
              break;
          }
          case SERIAL_KILLG:
          {
              Serial.print("NEXT GOAL");
              Serial.print("\n");
              encodeFrame(message, MANAGEMENT,NEXT_ORDER);
              XBee.send(message);
              break;
          }
          case SERIAL_CLEANG:
          {
              Serial.print("RESET GOALS");
              Serial.print("\n");
              encodeFrame(message, MANAGEMENT,RESET_ORDERS);
              XBee.send(message);
              break;
          }
  
          case SERIAL_GOTOA:
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
              encodeFrame(message, GOTOA,x,y,a_int,direction);
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
  
          case SERIAL_GOTO:
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
              encodeFrame(message, GOTO,x,y,direction);
              XBee.send(message);
  
              Serial.print(x);
              Serial.print("\n");
              Serial.print(y);
              Serial.print("\n");
  
              
              break;
          }
  
          case SERIAL_ROT:
          {
              Serial.print("ROT");
              Serial.print("\n");
              int16_t a_int;
  
               
              a_int =     atoi(args);
              
              encodeFrame(message, ROT,a_int);
              XBee.send(message);
  
              Serial.print(a_int);
              Serial.print("\n");
  
              
              break;
          }
  
          case SERIAL_ROTNOMODULO:
          {
              Serial.print("ROT NO MODULO");
              Serial.print("\n");
              long a_int;
              
              
              a_int =     atoi(args);
              args = strtok(0, ":"); 
              
              
              encodeFrame(message, ROTNOMODULO,a_int);
              XBee.send(message);
              Serial.print(a_int);
              Serial.print("\n");
              
  
              break;
          }
  
          case SERIAL_PIDLEFT:
          case SERIAL_PIDRIGHT:
          case SERIAL_PIDALL:
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
              
              
  
              
              if (commande == SERIAL_PIDLEFT)
              {
                  Serial.print("PID LEFT");
                  Serial.print("\n");
                  encodeFrame(message, PIDLEFT,p_int,i_int,d_int);
              }
              else if (commande == SERIAL_PIDRIGHT)
              {
                  Serial.print("PID RIGHT");
                  Serial.print("\n");
                  encodeFrame(message, PIDRIGHT,p_int,i_int,d_int);
              }
              else
              {
                Serial.print("PID ALL\n");
                encodeFrame(message, PIDALL,p_int,i_int,d_int);
              }
              Serial.println("about to send");
              XBee.send(message);
              Serial.println("sent");
              break;
          }
  
          case SERIAL_PWM:
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
              
              encodeFrame(message, PWM,l,r,t);
              XBee.send(message);
  
              Serial.print(l);
              Serial.print("\n");
              Serial.print(r);
              Serial.print("\n");
              Serial.print(t);
              Serial.print("\n");
  
              
              break;
          }
  
          case SERIAL_SET_POS:
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
              
              encodeFrame(message, SET_POS,x,y,a_int);
              XBee.send(message);
              
  
              Serial.print(x);
              Serial.print("\n");
              Serial.print(y);
              Serial.print("\n");
              Serial.print(a_int);
              Serial.print("\n");
  
              break;
          }
  
          case SERIAL_ACCMAX:
          {
              Serial.print("SET PARAM|");
              Serial.print("\n");
              int16_t r_int, s,a;
              
              s =     atoi(args);
              args = strtok(0, ":"); 
              r_int =     atoi(args);
              args = strtok(0, ":"); 
              a =     atoi(args);
              args = strtok(0, ":"); 
              
              encodeFrame(message, SET_PARAM,s,r_int,a);
              XBee.send(message);
  
              Serial.print(s);
              Serial.print("|");
              Serial.print(r_int);
              Serial.print("|");
              Serial.print(a);
              Serial.println("|");
              
              break;
          }
            
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
            Serial.println("HANDSHAKE");

            break;
        }

        case WHOAMI:
        {
            Serial.print("WHOAMI : ");
            switch(message[1])
            {
                case ALL_CAN_ADDR:
                    Serial.println("ALL");
                    break;

                case BBB_CAN_ADDR:
                    Serial.println("BBB");
                    break;

                case STM_CAN_ADDR:
                    Serial.println("STM");
                    break;

                case ARDUINO_CAN_ADDR:
                    Serial.println("ARD");
                    break;

                case ZIGBEE_CAN_ADDR:
                    Serial.println("ZIG B");
                    break;

                default:
                    break;
            }

            break;
        }

        case SET_MODE:
        {
            Serial.print("SET MODE|");
            break;
        }

        case SPD :
        {
            int linear, angular;
            unsigned int duration;
            linear   = message[1] << 8 | message[2];
            angular  = message[3] << 8 | message[4];
            duration = message[5] << 8 | message[6];
            
            Serial.print("|");
            Serial.print(linear);
            Serial.print("|");
            Serial.print(angular);
            Serial.print("|");
            Serial.print(duration);
            Serial.println("|");
            
            break;
        }

        case GET_CODER :
        {

            int left_wheel_dist, right_wheel_dist;
            
            left_wheel_dist  = message[1] << 8 | message[2];
            right_wheel_dist = message[3] << 8 | message[4];

            Serial.print("l wheel dist ");
            Serial.println(left_wheel_dist);
            Serial.print("r wheel dist");
            Serial.println(left_wheel_dist);
            
            break;
        }

        case MANAGEMENT :
        {
            
          switch(message[1])
          {
            case STOP :
            {
                Serial.println("STOP");
                break;
            }
    
            case START :
            {
                Serial.println("START");
                break;
            }
    
            case PAUSE :
            {
                Serial.println("PAUSE");
                break;
            }
    
            case RESUME :
            {
                Serial.println("RESUME");
                break;
            }
    
            case RESET_ID :
            {
                Serial.println("RESET ID");
                break;
            }
    
            case SETEMERGENCYSTOP :
            {
                Serial.println("EMG STOP");
                break;
            }
    
            case NEXT_ORDER :
            {
                Serial.println(" NEXT ORDER");
                break;
            }
    
            case RESET_ORDERS :
            {
                Serial.println("RST ORDERS");
                break;
            }
            default:
              break;
          }
          
          break;
        }

        case GOTOA :
        {
            int x, y, angle_int;
            unsigned char direction;
            
            x           = message[1] << 8 | message[2];
            y           = message[3] << 8 | message[4];
            angle_int   = message[5] << 8 | message[6];
            direction   = message[7];

            Serial.println("GOTOA");
            Serial.print("x ");
            Serial.println(x);
            Serial.print("y ");
            Serial.println(y);
            Serial.print("angle");
            Serial.println((float)angle_int/FLOAT_PRECISION);

            break;
        }

        case GOTO :
        {
            int x, y;
            unsigned char direction;
            
            x           = message[1] << 8 | message[2];
            y           = message[3] << 8 | message[4];
            direction   = message[5];
            
            Serial.println("GOTO");
            Serial.print("x ");
            Serial.println(x);
            Serial.print("y ");
            Serial.println(y);
            Serial.print("direction");
            Serial.println(direction);
            break;
        }

        case ROT :
        {
            int angle_int;
            angle_int = message[1] << 8 | message[2];
            Serial.println("ROT");
            Serial.print("angle ");
            Serial.println((float)angle_int/FLOAT_PRECISION);
            break;
        }

        case ROTNOMODULO :
        {
            int angle_int;
            angle_int = message[1] << 8 | message[2];
            Serial.println("ROT NO MODULO");
            Serial.print("angle ");
            Serial.println((float)angle_int/FLOAT_PRECISION);
            break;
        }

        case PIDLEFT :
        case PIDRIGHT :
        case PIDALL :
        {
            unsigned int p,i,d;
            p = message[1] | message[2];
            i = message[3] | message[4];
            d = message[5] | message[6];

            if ( mode == PIDLEFT)
            {
                Serial.println("PID LEFT");
            }
            else if (mode == PIDRIGHT)
            {
                Serial.println("PID RIGHT");
            }
            else
            {
                Serial.println("PID ALL");
            }

            Serial.print("p ");
            Serial.println(p);
            Serial.print("i ");
            Serial.println(i);
            Serial.print("d ");
            Serial.println(d);
            break;
        }

        case PWM :
        case CURRENT_PWM :
        {
            int pwm_left, pwm_right;
            
            pwm_left  = message[1] << 8 | message[2];
            pwm_right = message[3] << 8 | message[4];

            if (mode == PWM)
            {
                Serial.print("PWM    ");
            }
            else
            {
                Serial.print("PWM|");
            }

            //Serial.print("left ");
            Serial.print(pwm_left);
            Serial.print("|");
            Serial.print(pwm_right);
            Serial.print("  ");
            break;
        }

        case SET_POS :
        case CURRENT_POS :
        {
            int x, y, angle_int;
            
            x           = message[1] << 8 | message[2];
            y           = message[3] << 8 | message[4];
            angle_int   = message[5] << 8 | message[6];
            
            if (mode == SET_POS)
            {
                Serial.print("SET POS    ");
            }
            else
            {
                Serial.print ("POS|");
            }

            Serial.print(x);
            Serial.print("|");
            Serial.print(y);
            Serial.print("|");
            Serial.print(angle_int);
            Serial.print("  ");
            break;
        }

        case SET_PARAM :
        {
            unsigned int max_linear_speed, max_angular_speed,max_acceleration;
            
            max_linear_speed  = message[1] << 8 | message[2];
            max_angular_speed = message[3] << 8 | message[4];
            max_acceleration  = message[5] << 8 | message[6];

            Serial.println("SET PARAM");
            Serial.print("max lin spd ");
            Serial.println(max_linear_speed);
            Serial.print("max ang spd ");
            Serial.println(max_angular_speed);
            Serial.print("max acc");
            Serial.println(max_acceleration);
            
            break;
        }

        case CURRENT_SPD :
        {
            int linear_speed, left_wheel_speed, right_wheel_speed;
            linear_speed        = message[1] << 8 | message[2];
            left_wheel_speed    = message[3] << 8 | message[4];
            right_wheel_speed   = message[5] << 8 | message[5];
            
            Serial.print("SPD|");
            //Serial.print("lin   ");
            Serial.print(linear_speed);
            Serial.print("|");
            Serial.print(left_wheel_speed);
            Serial.print("|");
            Serial.println(right_wheel_speed);
            
            break;
        }

        case MOVE_PLIERS :
        {
            unsigned char level;
            level = message[1];

            Serial.println("MOVE PLIERS");
            Serial.print("lvl ");
            Serial.println(level);

        
            
            break;
        }

        case CLOSE_OPEN_PLIERS :
        {
            unsigned char order;
            order = message[1];
            if ( order > 0 )
            {
                Serial.println("CLOSE PLIERS");
            }
            else
            {
                Serial.println("OPEN PLIERS");
            }
            
            
            break;
        }

        case SONAR_DISTANCE :
        {
            unsigned char sonar_id;
            unsigned int distance;
            sonar_id = message[1];
            distance = message[2] << 8 | message[3];
            
            Serial.println("SONAR");
            Serial.print("id ");
            Serial.println(sonar_id);
            Serial.print("dist ");
            Serial.println(distance);
            
            break;
        }

        case THROW_BALLS :
        {
            
            Serial.println("THROW");
            
            break;
        }

        case OBJECT_ON_MAP:
        {
            Serial.print("OBJECT");
            uint8_t obj_ID;
            int16_t x,y;
            uint16_t radius;
            
            obj_ID = message[1];
            x      = message[2] << 8 | message[3];
            y      = message[4] << 8 | message[5];
            radius = message[6] << 8 | message[7];
            Serial.print("|");
            Serial.print(obj_ID);
            Serial.print("|");
            Serial.print(x);
            Serial.print("|");
            Serial.print(y);
            Serial.print("|");
            Serial.print(radius);
            Serial.println("|");
            break;
        }

        default:
            Serial.println("DATA");
            for (uint8_t i = 0; i< MSG_SIZE ; i++)
            {
              Serial.print("|");
              Serial.print(message[i]);
              
            }
            Serial.println("|");
            break;

    }
}