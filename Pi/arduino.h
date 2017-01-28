#if !defined( ARDUINO_H )
#define ARDUINO_H

#include <stdint.h>
#include <string>
#include <mutex>
#include "SerialPort.h"
#include "Pi_Arduino.h"

class arduino
{
public:

   arduino();

   ~arduino();
   
   
   
   void workLoop();
   
   void setRunning( bool amRunning );
      
   void setLMotorSpeed( int speed );
   void setRMotorSpeed( int speed );
   
private:

   void sendCmd();

   arduino( const arduino & copy );
   arduino &operator =( const arduino &rhs );
   
   SerialPort  serialPort;
   
   bool running;
   
   struct arduinoCmd cmd;
   
   std::mutex cmdLck;
   
   int validateSpeed( int speed );
   int getDirFromSpeed( int Speed );
};

#endif