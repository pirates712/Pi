#include "Pi_Arduino.h"

// Pins for motor control
const int rDir   = 7; 
const int rBrake = 2;
const int rPWM   = 5;

const int lDir   = 8;
const int lBrake = 4;
const int lPWM   = 6;

#define NUM_CMD_CHARS 10

struct arduinoCmd cmd;

boolean stringComplete = false;  // whether the string is complete

char cmdBuf[sizeof(arduinoCmd)];
unsigned int serCmdIdx = 0;
unsigned int expAuthCode = 55;

unsigned int lMotorRev = 0;
unsigned int rMotorRev = 0;

void setup() 
{
  // initialize serial:
  Serial.begin(115200);
  
  // initialize motor control pins
  pinMode(rDir, OUTPUT);
  pinMode(rBrake, OUTPUT);
  pinMode(rPWM, OUTPUT);
  
  pinMode(lDir, OUTPUT);
  pinMode(lBrake, OUTPUT);
  pinMode(lPWM, OUTPUT);
  
  digitalWrite( rDir, rMotorRev );
  digitalWrite( rBrake, 0 );

  digitalWrite( lDir, lMotorRev );
  digitalWrite( lBrake, 0 );
  
  memset( &cmd, 0, sizeof(cmd) );
  
}

void printCmd()
{
  //Serial.println("Got Command");
  
  if( expAuthCode == cmd.authCode )
  {
     ///Serial.println("Command is Valid");    
  }
  else
  {
     //Serial.print("Command is INVALID -");
     //Serial.print( cmd.authCode, DEC );
     //Serial.print("\n");
  }
  
  //Serial.print("LeftMotorVal- ");
  //Serial.print(cmd.lMotorVal, DEC);
  //Serial.print('\n');
  
  //Serial.print("RightMotorVal- ");
  //Serial.print(cmd.rMotorVal, DEC);
  //Serial.print('\n');
  
  //Serial.print("LeftMotorBrake- ");
  //Serial.print(cmd.lMotorBrake, DEC);
  //Serial.print('\n');
  
  //Serial.print("RightMotorBrake- ");
  //Serial.print(cmd.rMotorBrake, DEC);
  //Serial.print('\n');
  
}

void loop() 
{
  // print the string when a newline arrives:
  if (stringComplete) 
  {
    printCmd();
    
    char lMotorPwm = cmd.lMotorVal;
    char rMotorPwm = cmd.rMotorVal;
    
    lMotorRev = cmd.lMotorDir;
    rMotorRev = cmd.rMotorDir;
   
        
    /////////////// Update motor speeds //////////////////////////////
    
    digitalWrite( rDir, rMotorRev );
    digitalWrite( lDir, lMotorRev );
    
    analogWrite( lPWM, lMotorPwm );
    analogWrite( rPWM, rMotorPwm );

    digitalWrite( lBrake, cmd.lMotorBrake );
    digitalWrite( rBrake, cmd.rMotorBrake );

      
    /////////////////////////////////////////////////////////////////
       
    stringComplete = false;
    
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent()
{
  
  while (Serial.available() && serCmdIdx < sizeof(cmd))
  {
    // get the new byte:
    cmdBuf[serCmdIdx] = (char)Serial.read(); 
    serCmdIdx++;

  }
  if( serCmdIdx == sizeof(cmd) )
  {
     memcpy( &cmd, cmdBuf, sizeof(cmd) );
     stringComplete = true;
     memset( cmdBuf, 0, sizeof( cmdBuf ) );
     serCmdIdx = 0;
  } 
}


