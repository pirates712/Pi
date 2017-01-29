#include "arduino.h"
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "SerialPort.h"

#define THREAD_SLEEP_TIME (1000)
#define MAX_SPEED_MAG (255)

arduino::arduino()
{
    printf("Constructing Arduino Class!\n");
    serialPort.Open( "/dev/ttyACM0", "115200" );
    running = true;
}

arduino::~arduino()
{
   serialPort.Close();
}

void arduino::workLoop()
{
   struct arduinoCmd cmd;
   // Run as long as we are told. When we are told to stop
   // emty our command queue before actually stopping
   while( running || (cmdQueue.size() > 0 ))
   {
      cmd = cmdQueue.getAndPopFront();
      serialPort.Write( &cmd, sizeof(cmd) );
   }
   printf("%s: done!\n", __func__);
}

void arduino::setRunning( bool amRunning )
{
   if( amRunning != running )
   {
      printf("%s: Changing from %s to %s\n", __func__, (running?"RUNNING":"NOT RUNNING"), (amRunning?"RUNNING":"NOT RUNNING"));
   }
   running = amRunning;
}

void arduino::setMotorStates( unsigned int lMotorSpeed, unsigned int rMotorSpeed,
                              bool lMotorRev, bool rMotorRev,
                              bool lMotorBrake, bool rMotorBrake)
{
   // Don't add new commands if we aren't running!
   if( running )
   {
      struct arduinoCmd newCommand;

      newCommand.authCode = 55;

      newCommand.lMotorVal = validateSpeed( lMotorSpeed );
      newCommand.rMotorVal = validateSpeed( rMotorSpeed );

      newCommand.lMotorDir = (lMotorRev?1:0);
      newCommand.rMotorDir = (rMotorRev?1:0);

      newCommand.rMotorBrake = (lMotorBrake?1:0);
      newCommand.rMotorBrake = (rMotorBrake?1:0);

      cmdQueue.push_back(newCommand);
   }
}

unsigned int arduino::validateSpeed( unsigned int speed )
{
   int validSpeed = speed;
   // Validate range, should be -255 to 255
   if( validSpeed > MAX_SPEED_MAG )
   {
       validSpeed = MAX_SPEED_MAG;
   }

   return validSpeed;
}
