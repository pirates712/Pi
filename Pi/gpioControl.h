#pragma once
#include <string>

// Class for controlling a single GPIO pin.
// Source: http://www.hertaville.com/introduction-to-accessing-the-raspberry-pis-gpio-in-c.html
class gpioControl
{
public:
	gpioControl( std::string pin, std::string direction );
	~gpioControl();
	void testPrint();
	int  initialize();
	int  uninitialize();
	void setHigh();
	void setLow();
	int  getState( std::string &state, unsigned int debounceInMs = 20 );

private:
	std::string gpioPin;
	std::string gpioDirection;
};

