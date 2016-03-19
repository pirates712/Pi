#include "gpioControl.h"
#include "CommonDefs.h"
#include <stdio.h>
#include <iostream>
#include <fstream>



gpioControl::gpioControl( std::string pin, std::string direction )
{
	this->gpioPin = pin;
	if ( !direction.compare("in") || !direction.compare("out") )
	{
		this->gpioDirection = direction;
	}
	else
	{
		printf("Error! Invalid gpio direction\n");
	}
}

gpioControl::~gpioControl()
{
}

void gpioControl::testPrint()
{
	printf(" Hello, World! I'm a class! %s \n", EXPORT_STR);
}

int gpioControl::initialize()
{
	std::string export_str = std::string(EXPORT_STR);
	std::cout << export_str << std::endl;
	std::ofstream exportgpio(export_str.c_str());
	exportgpio << this->gpioPin;
	exportgpio.close();

	std::string exportGpioDir_str = std::string( GPIO_STR ) + std::string(this->gpioPin) + std::string("/direction");
	std::cout << exportGpioDir_str << std::endl;
	std::ofstream setGpioDir( exportGpioDir_str.c_str() );
	setGpioDir << this->gpioDirection;
	setGpioDir.close();

	return 0;
}

void gpioControl::setHigh()
{
	if( !( this->gpioDirection.compare( "out" ) ) )
	{
		std::string setval_str = std::string( GPIO_STR ) + std::string( this->gpioPin ) + std::string( "/value" );
		//std::cout << setval_str << std::endl;
		std::ofstream setGpioVal( setval_str.c_str() );
		setGpioVal << "1";
		setGpioVal.close();
	}
	else
	{
		std::cout << "ERROR: gpio " << this->gpioPin << " is not an output!" << std::endl;
	}
}

void gpioControl::setLow()
{
	if( !(this->gpioDirection.compare( "out" ) ) )
	{
		std::string setval_str = std::string( GPIO_STR ) + std::string( this->gpioPin ) + std::string( "/value" );
		//std::cout << setval_str << std::endl;
		std::ofstream setGpioVal( setval_str.c_str() );
		setGpioVal << "0";
		setGpioVal.close();
	}
	else
	{
		std::cout << "ERROR: gpio " << this->gpioPin << " is not an output!" << std::endl;
	}
}

int gpioControl::getState( std::string & state, unsigned int debounceInMs )
{
	if( !(this->gpioDirection.compare( "in" )) )
	{
		std::string getval_str = std::string( GPIO_STR ) + std::string( this->gpioPin ) + std::string( "/value" );
		//std::cout << getval_str << std::endl;
		std::ifstream getGpioVal( getval_str.c_str() );
		getGpioVal >> state;
		getGpioVal.close();
	}
	else
	{
		std::cout << "ERROR: gpio " << this->gpioPin << " is not an input!" << std::endl;
	}

}

int gpioControl::uninitialize()
{
	std::string unexport_str = std::string(UNEXPORT_STR);
	std::ofstream unexportgpio(unexport_str.c_str());
	unexportgpio << this->gpioPin;
	unexportgpio.close();

	return 0;
}
