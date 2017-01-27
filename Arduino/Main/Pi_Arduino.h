#if !defined( PI_ARDUINO_H )
#define PI_ARDUINO_H
struct arduinoCmd
{
    unsigned char authCode;
 
    unsigned char lMotorDir;
    unsigned char lMotorVal;
    unsigned char lMotorBrake;
 
    unsigned char rMotorDir;
    unsigned char rMotorVal;
    unsigned char rMotorBrake;
};
#endif