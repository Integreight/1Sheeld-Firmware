/*
 * ATmega_Firmata.c
 *
 * Created: 2/12/2013 9:43:21 AM
 *  Author: iMustafa
 */ 

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "uart.h"
#include "1sheelds_functions.h"
#include "pwm.h"
#include "firmata.h"
#include <util/delay.h>

#define TOTAL_PORTS             5
#define TOTAL_PINS              35
#define SYSEX_UART              0x0A
//variables declarations//
unsigned int BAUD_RATE;// for the hardware serial terminal
byte   isPulseInEnabled =0;

/* digital input ports */
byte reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
byte previousPINs[TOTAL_PORTS];     // previous 8 bits sent
// PIN means PORT for input

/* pins configuration */
byte pinConfig[TOTAL_PINS];         // configuration of every pin
byte portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else
int pinState[TOTAL_PINS];           // any value that has been written

char incomingString[MAX_UART_STRING_SIZE];
int stringPosition=0;
char tempChar;
char isUartStringStarted=0;

/*==============================================================================
 * FUNCTIONS
 *============================================================================*/


void outputPort(byte portNumber, byte portValue, byte forceSend)
{
  // pins not configured as INPUT are cleared to zeros
  portValue = portValue & portConfigInputs[portNumber];
  // only send if the value is different than previously sent
  if(forceSend || previousPINs[portNumber] != portValue) {
    Firmata.sendDigitalPort(portNumber, portValue);
    previousPINs[portNumber] = portValue;
  }
}

/* -----------------------------------------------------------------------------
 * check all the active digital inputs for change of state, then add any events
 * to the Serial output queue using Serial.print() */
void checkDigitalInputs(void)
{
  /* Using non-looping code allows constants to be given to readPort().
   * The compiler will apply substantial optimizations if the inputs
   * to readPort() are compile-time constants. */
  if (TOTAL_PORTS > 0 && reportPINs[0]) outputPort(0, readPort(0, portConfigInputs[0]), false);
  if (TOTAL_PORTS > 1 && reportPINs[1]) outputPort(1, readPort(1, portConfigInputs[1]), false);
  if (TOTAL_PORTS > 2 && reportPINs[2]) outputPort(2, readPort(2, portConfigInputs[2]), false);
  if (TOTAL_PORTS > 3 && reportPINs[3]) outputPort(3, readPort(3, portConfigInputs[3]), false);
  if (TOTAL_PORTS > 4 && reportPINs[4]) outputPort(4, readPort(4, portConfigInputs[4]), false);
  if (TOTAL_PORTS > 5 && reportPINs[5]) outputPort(5, readPort(5, portConfigInputs[5]), false);
  if (TOTAL_PORTS > 6 && reportPINs[6]) outputPort(6, readPort(6, portConfigInputs[6]), false);
  if (TOTAL_PORTS > 7 && reportPINs[7]) outputPort(7, readPort(7, portConfigInputs[7]), false);
  if (TOTAL_PORTS > 8 && reportPINs[8]) outputPort(8, readPort(8, portConfigInputs[8]), false);
  if (TOTAL_PORTS > 9 && reportPINs[9]) outputPort(9, readPort(9, portConfigInputs[9]), false);
  if (TOTAL_PORTS > 10 && reportPINs[10]) outputPort(10, readPort(10, portConfigInputs[10]), false);
  if (TOTAL_PORTS > 11 && reportPINs[11]) outputPort(11, readPort(11, portConfigInputs[11]), false);
  if (TOTAL_PORTS > 12 && reportPINs[12]) outputPort(12, readPort(12, portConfigInputs[12]), false);
  if (TOTAL_PORTS > 13 && reportPINs[13]) outputPort(13, readPort(13, portConfigInputs[13]), false);
  if (TOTAL_PORTS > 14 && reportPINs[14]) outputPort(14, readPort(14, portConfigInputs[14]), false);
  if (TOTAL_PORTS > 15 && reportPINs[15]) outputPort(15, readPort(15, portConfigInputs[15]), false);
}

// -----------------------------------------------------------------------------
/* sets the pin mode to the correct state and sets the relevant bits in the
 * two bit-arrays that track Digital I/O and PWM status
 */
void setPinModeCallback(byte pin, int mode)
{
  if (IS_PIN_DIGITAL(pin)) {
    if (mode == INPUT) {
      portConfigInputs[pin/8] |= (1 << (pin & 7));
    } else {
      portConfigInputs[pin/8] &= ~(1 << (pin & 7));
    }
  }
  pinState[pin] = 0;
  switch(mode) {
  case INPUT:
    if (IS_PIN_DIGITAL(pin)) {
      pinMode(pin, INPUT); // disable output driver
      digitalWrite(pin, LOW); // disable internal pull-ups
      pinConfig[pin] = INPUT;
    }
    break;
  case OUTPUT:
    if (IS_PIN_DIGITAL(pin)) {
      digitalWrite(pin, LOW); // disable PWM
      pinMode(pin, OUTPUT);
      pinConfig[pin] = OUTPUT;
    }
    break;
  case PWM:
    if (IS_PIN_PWM(pin)) {
      pinMode(pin, OUTPUT);
      analogWrite(pin, 0);
      pinConfig[pin] = PWM;
    }
    break;
  default:
    Firmata.sendString("Unknown pin mode"); // TODO: put error msgs in EEPROM
  }
  // TODO: save status to EEPROM here, if changed
}

void analogWriteCallback(byte pin, int value)
{
	if (pin < TOTAL_PINS) {
		switch(pinConfig[pin]) {
			
			case PWM:
			if (IS_PIN_PWM(pin))
			analogWrite(pin, value);
			pinState[pin] = value;
			break;
		}
	}
}

void digitalWriteCallback(byte port, int value)
{
	byte pin, lastPin, mask=1, pinWriteMask=0;

	if (port < TOTAL_PORTS) {
		// create a mask of the pins on this port that are writable.
		lastPin = port*8+8;
		if (lastPin > TOTAL_PINS) lastPin = TOTAL_PINS;
		for (pin=port*8; pin < lastPin; pin++) {
			// do not disturb non-digital pins (eg, Rx & Tx)
			if (IS_PIN_DIGITAL(pin)) {
				// only write to OUTPUT and INPUT (enables pullup)
				// do not touch pins in PWM, ANALOG, SERVO or other modes
				if (pinConfig[pin] == OUTPUT || pinConfig[pin] == INPUT) {
					pinWriteMask |= mask;
					pinState[pin] = ((byte)value & mask) ? 1 : 0;
				}
			}
			mask = mask << 1;
		}
		writePort(port, (byte)value, pinWriteMask);
	}
}

void reportDigitalCallback(byte port, int value)
{
	if (port < TOTAL_PORTS) {
		reportPINs[port] = (byte)value;
	}

}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void sysexCallback(byte command, byte argc, byte *argv)
{
           byte mode;
           byte slaveAddress;
           byte slaveRegister;
           byte data;
           byte newData [argc/2];
  unsigned int  delayTime; 
           int  s16DataCounter;
		       
		   
		        
  switch(command) {
	  case SYSEX_UART:
	  for (int s16DataCounter = 0; s16DataCounter < argc; s16DataCounter+=2) // run over and over
	  {
		  newData[s16DataCounter/2]=(argv[s16DataCounter]|(argv[s16DataCounter+1]<<7));
		  UartTx0(newData[s16DataCounter/2]);
	  }
	  //for(s16DataCounter = 0; s16DataCounter < argc ;s16DataCounter++)
	  //{
	//	  UartTx0(argv[s16DataCounter]);//mySerial.write(argv[s16DataCounter]);
	 // }
	  break;
	  case CAPABILITY_QUERY:
	  UartTx1(START_SYSEX);
	  UartTx1(CAPABILITY_RESPONSE);
	  for (byte pin=0; pin < TOTAL_PINS; pin++) {
		  if (IS_PIN_DIGITAL(pin)) {
			  UartTx1((byte)INPUT);
			  
			  UartTx1(1);
			  UartTx1((byte)OUTPUT);
			  UartTx1(1);
		  }
	      if (IS_PIN_PWM(pin)) {
		      UartTx1(PWM);
		      UartTx1(8);
	      }
		  UartTx1(127);
	  }
	  UartTx1(END_SYSEX);
	  break;
	
	 case PIN_STATE_QUERY:
	 if (argc > 0) {
		 byte pin=argv[0];
		 UartTx1(START_SYSEX);
		 UartTx1(PIN_STATE_RESPONSE);
		 UartTx1(pin);
		 if (pin < TOTAL_PINS) {
			 UartTx1((byte)pinConfig[pin]);
			 UartTx1((byte)pinState[pin] & 0x7F);
			 if (pinState[pin] & 0xFF80) UartTx1((byte)(pinState[pin] >> 7) & 0x7F);
			 if (pinState[pin] & 0xC000) UartTx1((byte)(pinState[pin] >> 14) & 0x7F);
		 }
		 UartTx1(END_SYSEX);
	 }
	 break;
	/*  case UART_COMMAND:  //0x80
	  {
		  if(argv[0]==UART_BEGIN)
		  {
			  if(argv[1]==0x00) BAUD_RATE=1200;
			  else  if(argv[1]==0x01) BAUD_RATE =2400;
			  else  if(argv[1]==0x02) BAUD_RATE =4800;
			  else  if(argv[1]==0x03) BAUD_RATE =9600;
			  else  if(argv[1]==0x04) BAUD_RATE =14400;
			  else  if(argv[1]==0x05) BAUD_RATE =19200;
			  else  if(argv[1]==0x06) BAUD_RATE =28800;
			  else  if(argv[1]==0x07) BAUD_RATE =38400;
			  else  if(argv[1]==0x08) BAUD_RATE =57600;
			  else  if(argv[1]==0x09) BAUD_RATE =115200;
			  else BAUD_RATE =57600;
			  UartInit(BAUD_RATE);

		  }
		  else if(argv[0]==UART_END) Serial.end();
		  
	  }break;
	case UART_DATA://0x82
	{
		byte newData [argc/2];
		for (int i = 0; i < argc; i+=2) // run over and over
		{
			newData[i/2]=(argv[i]|(argv[i+1]<<7));
			Serial.write(newData[i/2]);
		}
		
	}
	break; */ // todo later
	/*case PULSE_IN_INIT:  //0x86
	{
		delete(convertPWM);
		byte pin_no;
		if(argv[0]==0x03)
		pin_no=3;
		else  if(argv[0]==0x05)
		pin_no =5;
		
		convertPWM   = new  ConvertPWM(pin_no); // make ana object of ConvertPWM with pin 8 to read pulse in
		
		if(argv[1]==0x00)
		isPulseInEnabled =1;
		else  if(argv[1]==0x04)
		isPulseInEnabled =0;
		
	}break; */ 
	}
}
void systemResetCallback()
{	
	for (byte i=0; i < TOTAL_PORTS; i++) {
		reportPINs[i] = false;      // by default, reporting off
		portConfigInputs[i] = 0;	// until activated
		previousPINs[i] = 0;
	}
	
	for (byte i=0; i < TOTAL_PINS; i++)
    {
		setPinModeCallback(i, OUTPUT);
	}
}
int main(void)
{
    uint8 duty=0;
	byte  u8SoftData[MAX_DATA_BYTES];
	int   s16DataLength, s16DataCounter;
	
	
	sei(); // global interrupt enable 
	Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
	Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
	Firmata.attach(REPORT_DIGITAL, reportDigitalCallback);
	Firmata.attach(SET_PIN_MODE, setPinModeCallback);
	Firmata.attach(START_SYSEX, sysexCallback);
	Firmata.attach(SYSTEM_RESET, systemResetCallback);

	Firmata.begin(57600);
	UartInit(0, BAUD_57600);
	systemResetCallback();  // reset to default config
	UartTx1('X');
	while (1) // the super loop!
	{
		
		s16DataLength  = serial0_Avilable();//mySerial.available();
		if(s16DataLength > 0)
		{
			
			for(s16DataCounter = 0; s16DataCounter < s16DataLength; s16DataCounter++)
			{
				u8SoftData[s16DataCounter] = UartRx0();//mySerial.read();
			}
			
			/* if data received transmit to HW UART of firmata by sysex */
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
		
		checkDigitalInputs();
		while(serial1_Avilable()>0)
          {
             
             Firmata.processInput();
          }
          
		

  }

}