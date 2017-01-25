

// Pins for motor control
const int rDir   = 7; 
const int rBrake = 2;
const int rPWM   = 5;

const int lDir   = 8;
const int lBrake = 4;
const int lPWM   = 6;


#define NUM_CMD_CHARS 10

boolean stringComplete = false;  // whether the string is complete

char cmd[NUM_CMD_CHARS];
unsigned int serCmdIdx = 0;
unsigned int expAuthCode = 55;

unsigned int lMotorRev = 0;
unsigned int rMotorRev = 0;

void setup() 
{
  // initialize serial:
  Serial.begin(9600);
  
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
  
}

void printCmd()
{
  Serial.println("Got Command");
  
  if( expAuthCode == cmd[0] )
  {
     Serial.println("Command is Valid");    
  }
  else
  {
     Serial.println("Command is INVALID");
     return;
  }
  
  Serial.print("LeftMotorVal- ");
  Serial.print(cmd[1], DEC);
  Serial.print('\n');
  
  Serial.print("RightMotorVal- ");
  Serial.print(cmd[2], DEC);
  Serial.print('\n');
  
  Serial.print("LeftMotorBrake- ");
  Serial.print(cmd[3], DEC);
  Serial.print('\n');
  
  Serial.print("RightMotorBrake- ");
  Serial.print(cmd[4], DEC);
  Serial.print('\n');
  
}

void loop() 
{
  // print the string when a newline arrives:
  if (stringComplete) 
  {
    printCmd();
    
    char lMotorPwm = cmd[1];
    char rMotorPwm = cmd[2];
    
    if( lMotorPwm < 0 )
    {
      lMotorRev = 1;
      lMotorPwm = abs(lMotorPwm);
    }
    else
    {
      lMotorRev = 0;
    }
    
    if( rMotorPwm < 0 )
    {
      rMotorRev = 1;
      rMotorPwm = abs(rMotorPwm);
    }
    else
    {
      rMotorRev = 0;
    }
    
    
        
    /////////////// Update motor speeds //////////////////////////////
    
    digitalWrite( rDir, rMotorRev );
    digitalWrite( lDir, lMotorRev );
    
    analogWrite( lPWM, lMotorPwm );
    analogWrite( rPWM, rMotorPwm );

    digitalWrite( lBrake, cmd[3] );
    digitalWrite( rBrake, cmd[4] );

      
    /////////////////////////////////////////////////////////////////
       
    stringComplete = false;
    memset( cmd, 0, sizeof( cmd ) );
    serCmdIdx = 0;
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
  
  while (Serial.available() && serCmdIdx < NUM_CMD_CHARS)
  {
    // get the new byte:
    cmd[serCmdIdx] = (char)Serial.read(); 
    serCmdIdx++;

  }
  if( serCmdIdx == NUM_CMD_CHARS )
  {
     stringComplete = true;
  } 
}


