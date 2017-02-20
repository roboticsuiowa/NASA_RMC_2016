#include <SoftwareSerial.h>
#include <Sabertooth.h>
#include <DualVNH5019MotorShield.h>

/*********************************************/
/* VARIABLES AND DEFINES FOR MESSAGE PARSING */
/*********************************************/
/* message format:
 * [Start Char][Motor Number][Direction][Power]
 * the start char, motor number, and direction will all be 1 byte each
 * and the power will always be a 3 digit integer (zero padded on left).
 * The current motor mappings are listed below in the defines.
 * ex. 
 * "!20100" means motor 2 backwards at power=100 
 * "!41012" means motor 4 forwards at power=12
 */

// Define what motors do what
#define DRIVE_TRAIN_RIGHT '0'
#define DRIVE_TRAIN_LEFT '1'
#define DIGGER '2'
#define BUCKET '3'
#define PUSHER '4'

// Define what the directions are
#define FORWARD '1'
#define BACKWARD '0'

// Define what the first character of a valid message is
#define START_CHAR '!'
#define STOP_ALL 's'

// variables for storing message parts
char motor_byte;
char dir_byte;
char power_bytes[4];

// The current byte being read
char in_byte;

// What part of the message we are reading
int read_index = 0;

// Wether or not we have a complete message or not
bool message_done = false;

/***********************************************/
/* VARIABLES AND DEFINES FOR MOTOR CONTROLLING */
/***********************************************/

SoftwareSerial SWSerial_Drive(NOT_A_PIN,14);
SoftwareSerial SWSerial_Dig(NOT_A_PIN,16);
Sabertooth ST_Drive(128,SWSerial_Drive);
Sabertooth ST_Dig(128,SWSerial_Dig);
DualVNH5019MotorShield MD_Bucket;

/*********/
/* SETUP */
/*********/

void setup()
{
  power_bytes[3] = '\0';

  Serial.begin(9600);
  
  SWSerial_Drive.begin(9600);
  SWSerial_Dig.begin(9600);

  ST_Drive.autobaud();
  ST_Drive.setRamping(55);
  ST_Dig.autobaud();
  ST_Dig.setRamping(55);

  MD_Bucket.init();
}
/********/
/* LOOP */
/********/

void loop()
{
	/* what for new data to become available */
  while (Serial.available() == 0)  {}
  in_byte = Serial.read();
  /* if the new data was the beginning of a message then reset everything */
  /* if the new byte says stop, then stop everything and reset */
  if(in_byte == STOP_ALL)
  {
  	allStop();
  	read_index = 0;
  	message_done = false;
  }
  else if(in_byte == START_CHAR)
  {
  	read_index = 1;
  	message_done = false;
  }
  /* read the next byte into motor_byte */
  else if(read_index == 1)
  {
  	motor_byte = in_byte;
  	read_index = 2; // start looking for dir_byte
  }
  /* read the next byte into dir_byte */
  else if(read_index == 2)
  {
  	dir_byte = in_byte;
  	read_index = 3; // start looking for power_bytes
  }
  /* read the next three bytes into power_bytes */
  else if(read_index == 3)
  {
  	power_bytes[0] = in_byte;
  	read_index = 4;
  }
  else if(read_index == 4)
  {
  	power_bytes[1] = in_byte;
  	read_index = 5;
  }
  else if(read_index == 5)
  {
  	power_bytes[2] = in_byte;
  	read_index = 0; // stop looking for new data bytes (only start byte)
  	message_done = true; // let the program know that the message isready
  }

  /* if we have a full message, process it */
  if(message_done)
  {
  	message_done = false;
  	/* convert power_bytes and dir_byte into a signed integer */
  	int p_level = atoi(power_bytes);
  	if(dir_byte == BACKWARD) { p_level = -1*p_level; }

  	/* set the power level of the correct motor */
  	if(motor_byte == DRIVE_TRAIN_RIGHT)
  	{
  		ST_Drive.motor(1,-1*p_level);
  	}
  	if(motor_byte == DRIVE_TRAIN_LEFT)
  	{
  		ST_Drive.motor(2,-1*p_level);
  	}
  	if(motor_byte == DIGGER)
  	{
  		ST_Dig.motor(1,p_level);
  	}
  	if(motor_byte == BUCKET)
  	{
  		MD_Bucket.setSpeeds(p_level,p_level);
  	}
    if(motor_byte == PUSHER)
    {
      ST_Dig.motor(2,p_level);
    }
  }
} // loop

void allStop()
{
	ST_Drive.motor(1,0);
	ST_Drive.motor(2,0);
	ST_Dig.motor(1,0);
	ST_Dig.motor(2,0);
	MD_Bucket.setSpeeds(0,0);
}
