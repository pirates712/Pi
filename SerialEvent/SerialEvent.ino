/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */

#define NUM_CMD_CHARS 10

boolean stringComplete = false;  // whether the string is complete

char cmd[NUM_CMD_CHARS];
unsigned int serCmdIdx = 0;

void setup() 
{
  // initialize serial:
  Serial.begin(9600);
}

void loop() 
{
  // print the string when a newline arrives:
  if (stringComplete) 
  {
    Serial.println("Got Command");
    for( int i = 0; i < NUM_CMD_CHARS; i++ )
    {
       Serial.print(cmd[i], DEC);
       Serial.print(" ");    
      
    }
    
    Serial.print('\n'); 
   
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


