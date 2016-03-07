/*

  Project:       1Sheeld Firmware 
  File:          main.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "onesheeld.h"

int main(void)
{ 
	initialization();
	catchTimeForSomeVariables();
	while (1)
	{		
		newMillis = millis();
		checkDigitalPinStatus();
		#ifdef ONE_SHEELD_PLUS
		checkDataAvailabilityInRx0Buffer(); 
		checkArduinoRx0BufferSpace();
		#endif // ONE_SHEELD_PLUS
		processDataFromApp();
		checkBluetoothResetResponse();
		checkAppConnection();
		sendDataToApp();
	}
}

