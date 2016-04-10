/*

  Project:       1Sheeld Firmware 
  File:          main.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/
#include "stdint.h"
#include "config.h"
#include "onesheeld.h"

int main(void)
{ 
	initialization();
	catchTimeForSomeVariables();
	while (1)
	{		
		newMillis = millis();
		checkDigitalPinStatus();
		#ifdef PLUS_BOARD
		checkArduinoRx0BufferSpace();
		#endif // PLUS_BOARD
		processDataFromApp();
		checkBluetoothResetResponse();
		checkAppConnection();
		sendDataToApp();
	}
}

