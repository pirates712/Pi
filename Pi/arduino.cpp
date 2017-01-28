#include "arduino.h"
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "SerialPort.h"

#define THREAD_SLEEP_TIME (100000)
#define MAX_SPEED_MAG (255)

arduino::arduino()
{
    printf("Constructing Arduino Class!\n");
    serialPort.Open( "/dev/ttyACM0", "9600" );
    memset( &cmd, 0, sizeof(cmd));
    cmd.authCode = 55;
    running = true;
}

arduino::~arduino()
{
   serialPort.Close();
}

void arduino::workLoop()
{
   while( running )
   {
      cmdLck.lock();
      sendCmd();
      cmdLck.unlock();
      usleep( THREAD_SLEEP_TIME );
   }
   printf("%s: done!\n", __func__);
}

void arduino::setRunning( bool amRunning )
{
   if( amRunning != running )
   {
      printf("%s: Changing from %s to %s\n", __func__, (running?"RUNNING":"NOT RUNNING"), (amRunning?"RUNNING":"NOT RUNNING"));
   }
   // Sleep for 3x thread sleep time to ensure last command is issued
   usleep(3*THREAD_SLEEP_TIME);
   running = amRunning;
}

void arduino::sendCmd()
{
    serialPort.Write( &cmd, sizeof(cmd) );
}

// Takes a signed int representing speed, where negative means reverse
void arduino::setLMotorSpeed( int speed )
{
   int validSpeed = validateSpeed( speed );
   int dir        = getDirFromSpeed( speed );
   
   printf("Setting lMotor Speed to %d!\n", validSpeed);
   
   cmdLck.lock();
   cmd.lMotorVal = validSpeed;
   cmd.lMotorDir = dir;
   cmdLck.unlock();
}

// Takes a signed int representing speed, where negative means reverse
void arduino::setRMotorSpeed( int speed )
{
   int validSpeed = validateSpeed( speed );
   int dir        = getDirFromSpeed( speed );
   
   printf("Setting rMotor Speed to %d!\n", validSpeed);
   
   cmdLck.lock();
   cmd.rMotorVal = validSpeed;
   cmd.rMotorDir = dir;
   cmdLck.unlock();
}

int arduino::validateSpeed( int speed )
{
   int validSpeed = speed;
   
   // Validate range, should be -255 to 255
   if( validSpeed > MAX_SPEED_MAG )
   {
       validSpeed = MAX_SPEED_MAG;
   }
   else if( validSpeed < -MAX_SPEED_MAG)
   {
       validSpeed = -MAX_SPEED_MAG;
   }
   
   if( validSpeed < 0 )
   {
      validSpeed = abs(validSpeed);
   }

   return validSpeed;
}

int arduino::getDirFromSpeed( int speed )
{
   int dir = 0;
    
   if( speed < 0 )
   {
      dir = 1;
   }
   
   return dir;
}