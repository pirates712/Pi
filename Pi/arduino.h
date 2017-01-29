#if !defined( ARDUINO_H )
#define ARDUINO_H

#include <stdint.h>
#include <string>
#include "SerialPort.h"
#include "Pi_Arduino.h"
#include "threadSafeList.h"

class arduino
{
public:

   arduino();

   ~arduino();



   void workLoop();

   void setRunning( bool amRunning );

   void setMotorStates( unsigned int lMotorSpeed, unsigned int rMotorSpeed,
                        bool lMotorRev, bool rMotorRev,
                        bool lMotorBrake, bool rMotorBrake);

private:

   arduino( const arduino & copy );
   arduino &operator =( const arduino &rhs );

   SerialPort  serialPort;

   bool running;

   threadSafeList<struct arduinoCmd> cmdQueue;

   unsigned int validateSpeed( unsigned int speed );
};

#endif
