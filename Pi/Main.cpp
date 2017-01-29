#include "gpioControl.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <thread>
#include "arduino.h"
#include "net_connect.h"

#define NUM_CMD_CHARS 10

void runArduinoThread( arduino * a )
{
   a->workLoop();
}


main()
{
   printf("Starting up!\n");

   net_connect mTestSocket(265, 9595);


#if 0
   gpioControl myGpioController( "4", "out" );
   gpioControl myGpioInputController( "17", "in" );

   myGpioController.testPrint();

   std::string inputState = "0";
#endif
   arduino mArduino;
   std::thread arduinoThread( runArduinoThread, &mArduino );

   mArduino.setMotorStates( 0, 0, 0, 0, 0, 0 );

   mTestSocket.waitForConnection();

   // Once connected, start receiving data from the client
   std::thread rxThread( start_receive, &mTestSocket );

   while(mTestSocket.isConnected())
   {
      char * buf = mTestSocket.getBuffer();
      printf("MAIN -- Here is the message: %d %d %d %d\n",buf[0], buf[1], buf[2], buf[3]);
      mArduino.setMotorStates( buf[0], buf[0], 0, 0, 0, 0 );
      free( buf );
   }
   //mArduino.setLMotorSpeed( 80 );
   //mArduino.setRMotorSpeed( 80 );


   mArduino.setRunning( false );
   arduinoThread.join();
   rxThread.join();

      //serialPort.Read( rx, sizeof( rx ) );
      //rx[9] = '.';
      //printf( "ONSTART %u : %u : %u : %u : %u : %u : %u : %u : %u : %u END\n", rx[0], rx[1], rx[2], rx[3], rx[4], rx[5], rx[6], rx[7], rx[8], rx[9] );
      //printf( "ONSTART %u END\n", rx[0] );
      //std::cout << std::string( rx ) << std::endl;
      //memset( rx, 0, sizeof( rx ) );

      //test[0] = 'o';
      //test[1] = 8;
      //test[9] = '\n';

      //serialPort.Write( test, sizeof( test ) );
      //usleep( 10 );
      //serialPort.Read( rx, sizeof( rx ) );
      //printf( "ONSTART %u : %u : %u : %u : %u : %u : %u : %u : %u : %u END\n", rx[0], rx[1], rx[2], rx[3], rx[4], rx[5], rx[6], rx[7], rx[8], rx[9] );
      //printf( "ONSTART %u END\n", rx[0] );
      //std::cout << std::string( rx ) << std::endl;
      //memset( rx, 0, sizeof( rx ) );

   /*
	myGpioController.initialize();
	myGpioInputController.initialize();

   char * speed = "/dev/ttyACM0";

	for( int i = 0; i < 1000; i++ )
	{
		//myGpioController.setHigh();
		usleep( 100000 );
		myGpioInputController.getState( inputState );

		if( inputState.compare( "0" ) )
		{
			myGpioController.setHigh();
		}
		else
		{
			myGpioController.setLow();
		}


		//myGpioController.setLow();
		//usleep( 100000 );
	}

	myGpioController.uninitialize();*/

	printf("done!\n");

}
