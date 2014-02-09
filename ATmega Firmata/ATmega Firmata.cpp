/*
 * ATmega_Firmata.c
 *
 * Created: 2/12/2013 9:43:21 AM
 *  Author: iMustafa
 */ 

#define  F_CPU 16000000UL //
#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "uart.h"
#include "1sheelds_functions.h"
#include "pwm.h"
#include "firmata.h"
#include <util/delay.h>



int main(void)
{

    uint8 duty=0;
	byte  u8SoftData[MAX_DATA_BYTES];
	int   s16DataLength, s16DataCounter;

	sei(); // global interrupt enable 
	//Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
	//Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
	//Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
	//Firmata.attach(REPORT_DIGITAL, reportDigitalCallback);
	//Firmata.attach(SET_PIN_MODE, setPinModeCallback);
	//Firmata.attach(START_SYSEX, sysexCallback);
	//Firmata.attach(SYSTEM_RESET, systemResetCallback);

	Firmata.begin(57600);
	
	Firmata.systemResetCallback();  // reset to default config

	//UartInit(0, BAUD_57600);
	while (1) // the super loop!
	{
		/*
		s16DataLength  = serial0_Avilable();//mySerial.available();
		if(s16DataLength > 0)
		{
			
			for(s16DataCounter = 0; s16DataCounter < s16DataLength; s16DataCounter++)
			{
				u8SoftData[s16DataCounter] = UartRx0();//mySerial.read();
			}
			
			// if data received transmit to HW UART of firmata by sysex 
			Firmata.sendSysex(SYSEX_UART, s16DataLength, u8SoftData);
		}
		/*if (Serial.available()>0)
		{
			tempChar=Serial.read();
			if(tempChar==STX&&!isUartStringStarted){
				memset(&incomingString, 0, MAX_UART_STRING_SIZE );
				stringPosition=0;
				isUartStringStarted=1;
			}
			else if(tempChar==ETX&&isUartStringStarted){
				incomingString[stringPosition]='\0';
				Firmata.sendString(UART_DATA,incomingString);
				isUartStringStarted=0;
			}
			else if(isUartStringStarted){
				incomingString[stringPosition]=tempChar;
				stringPosition++;
			}
			else{
				Firmata.sendSysexDataByte(UART_DATA,tempChar);
			}
		}
		if(isPulseInEnabled)
		{
			
			byte analogData[2];
			long value =convertPWM->Convert();
			analogData [0]=(value & B01111111);
			analogData[1]=(value >> 7 & B01111111);
			//Firmata.sendSysex(PULSE_IN_DATA,2,analogData); // convert to analog and send it via serial comm
			Firmata.sendByte(START_SYSEX);
			Firmata.sendByte(0x88);//command
			Firmata.sendByte(analogData [0]);
			Firmata.sendByte(analogData [1]); // convert to analog and send it via serial comm
			Firmata.sendByte(END_SYSEX);
			
			
		}*/

		Firmata.checkDigitalInputs();
		while(Firmata.available()>0)
          {
             
             Firmata.processInput();
          }
          
		

  }

}