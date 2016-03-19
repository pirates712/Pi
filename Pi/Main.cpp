#include "gpioControl.h"
#include "SerialPort.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

main()
{
	printf("Starting up!\n");

	gpioControl myGpioController( "4", "out" );
	gpioControl myGpioInputController( "17", "in" );
   SerialPort  serialPort;
	myGpioController.testPrint();

	std::string inputState = "0";

   char test[10];
   char rx[1];

   //test[0] = 'i';
   //test[1] = 100;
   test[9] = '\n';

   memset( rx, 0, sizeof( rx ) );

   serialPort.Open( "/dev/ttyACM0", "9600" );

   for( int i = 0; i < 10; i++ )
   {

      test[0] = 'i';
      test[1] = 100;
      test[9] = '\n';

      serialPort.Write( test, sizeof( test ) );
      usleep( 10 );
      serialPort.Read( rx, sizeof( rx ) );
      //rx[9] = '.';
      //printf( "ONSTART %u : %u : %u : %u : %u : %u : %u : %u : %u : %u END\n", rx[0], rx[1], rx[2], rx[3], rx[4], rx[5], rx[6], rx[7], rx[8], rx[9] );
      printf( "ONSTART %u END\n", rx[0] );
      //std::cout << std::string( rx ) << std::endl;
      memset( rx, 0, sizeof( rx ) );

      usleep( 500000 );

      test[0] = 'o';
      test[1] = 8;
      test[9] = '\n';

      serialPort.Write( test, sizeof( test ) );
      usleep( 10 );
      serialPort.Read( rx, sizeof( rx ) );
      //printf( "ONSTART %u : %u : %u : %u : %u : %u : %u : %u : %u : %u END\n", rx[0], rx[1], rx[2], rx[3], rx[4], rx[5], rx[6], rx[7], rx[8], rx[9] );
      printf( "ONSTART %u END\n", rx[0] );
      //std::cout << std::string( rx ) << std::endl;
      memset( rx, 0, sizeof( rx ) );

      usleep( 500000 );

   }
   
   serialPort.Close();



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