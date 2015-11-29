/*

  Project:       1Sheeld Firmware 
  File:          ATmega Firmata.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "1sheelds_functions.h"
#include "firmata.h"

int main(void)
{ 
	setupMillisTimers();
	sei();
	initFirmata();
	setUnusedPinsAsOutput();
	requestBluetoothReset();
	bluetoothResponseMillis=millis();
	isAliveMillis=millis();
	sentFramesMillis=millis();
	setupUartLeds();
    initUart(0);
	sendIsAlive();
	while (1)
	{		
		checkDigitalInputs();
		checkDataAvailabilityInRx0Buffer();		
		processDataFromApp();
		newMillis = millis();
		checkBluetoothResetResponse();
		checkArduinoRx0BufferSpace();
		checkAppConnection();
		sendDataToApp();
	}
}

