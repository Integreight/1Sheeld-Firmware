/*

  Project:       1Sheeld Firmware 
  File:          1sheelds_functions.cpp

  Compiler:      avr-gcc 3.4.2

  Author:        Integreight
                 
  Date:          2014.5

*/

#include "1sheelds_functions.h"

void setPinMode(uint8 pin , uint8 pinMode)
{
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;
		
	
	if(port == NOT_A_PIN) 
	{
		return;
	}
	
	cfg.pID =  portModeRegister(port);
	
	if(pinMode == INPUT)
	{
	    setPinModeInput(&cfg, bit);
	}
	else 
	{		
	    setPinModeOutput(&cfg, bit);
	}
	
}

void turnOffPWM(uint8 timer)
{
	
	
	switch(timer)
	{
		case TIMER_00 :
		TCCR0&=3;//for millis
		break;
		
		case TIMER_1A:
		CLR_BIT(TCCR1A ,COM1A1);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
		break;
		
		case TIMER_1B:
		CLR_BIT(TCCR1A ,COM1B1);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
		break;
		
		case TIMER_02:
		TCCR2 &= 0x03;
		break;
		
		case TIMER_3A:
		CLR_BIT(TCCR3A ,COM3A1);
		TCCR3B|=(1<<WGM32)|(1<<CS30);
		break;
		
		case TIMER_3B:
		CLR_BIT(TCCR3A ,COM3B1);
		TCCR3B|=(1<<WGM32)|(1<<CS30);
		break;
		
		default:
		break;
		
	}
}

uint8  digitalRead(uint8 pin)
{
	uint8 timer = digitalPinToTimer(pin);
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;
	
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	if(port == NOT_A_PIN)
	{
		return 0;
	}
	
	cfg.pID = (unsigned int)portModeRegister(port);
	
	return getPinValue(cfg.pID,bit);
			
}

void resetBluetooth()
{
	//bt reset
	SET_BIT(DDRE,0);
	SET_BIT(PORTE,0);
	_delay_ms(5);
	CLR_BIT(PORTE,0);
}
void   digitalWrite(uint8 pin, uint8 value)
{
	uint8 timer = digitalPinToTimer(pin);
	uint8 bit = digitalPinToBitMask(pin);
	uint8 port = digitalPinToPort(pin);
	t_SetPortCfg cfg;

	if (timer != NOT_ON_TIMER) turnOffPWM(timer);
	
	if(port == NOT_A_PORT)
	{
		return;
	}
	
	cfg.pID = (unsigned int)portModeRegister(port);
	
	setPinValue(value,cfg.pID,bit);
}


void analogWrite(uint8 pin, int val)
{
	uint8 timer = 0xff;
	setPinMode(pin, OUTPUT);
	
	if ((val == 0) || (val <0))
	{
		digitalWrite(pin, LOW);

	}
	else if ((val > 255) || (val == 255))
	{
		digitalWrite(pin, HIGH);

	}
	else
	{
		timer = digitalPinToTimer(pin);
		initPwm(timer);
		setPwmDutyCycle((uint8)val, timer);

	}
	
}


uint8 readPort(byte port, byte bitmask)
{
	unsigned char out=0, pin=port*8;
	if ((IS_PIN_DIGITAL(pin+0)) && (bitmask & 0x01) && ( digitalRead(pin+0))) out |= 0x01;
	if ((IS_PIN_DIGITAL(pin+1)) && (bitmask & 0x02) && ( digitalRead(pin+1))) out |= 0x02;
	if ((IS_PIN_DIGITAL(pin+2)) && (bitmask & 0x04) && ( digitalRead(pin+2))) out |= 0x04;
	if ((IS_PIN_DIGITAL(pin+3)) && (bitmask & 0x08) && ( digitalRead(pin+3))) out |= 0x08;
	if ((IS_PIN_DIGITAL(pin+4)) && (bitmask & 0x10) && ( digitalRead(pin+4))) out |= 0x10;
	if ((IS_PIN_DIGITAL(pin+5)) && (bitmask & 0x20) && ( digitalRead(pin+5))) out |= 0x20;
	if ((IS_PIN_DIGITAL(pin+6)) && (bitmask & 0x40) && ( digitalRead(pin+6))) out |= 0x40;
	if ((IS_PIN_DIGITAL(pin+7)) && (bitmask & 0x80) && ( digitalRead(pin+7))) out |= 0x80;
	return out;
}

void writePort(byte port, byte value, byte bitmask)
{
	byte pin=port*8;
	
	if (port==0)
	{
       bitmask&=0xfc; // don't touch uart pins (0,1)!!
    }
	else if (port==2)
	{
	   bitmask&=0x7f; // don't touch uart pins (23)!!
	}
	else if (port==3)
	{
	   bitmask&=0xfe; // don't touch uart pins (24)!!
	}

	if ((bitmask & 0x01)) digitalWrite((pin+0), (value & 0x01));
	if ((bitmask & 0x02)) digitalWrite((pin+1), (value & 0x02));
	if ((bitmask & 0x04)) digitalWrite((pin+2), (value & 0x04));
	if ((bitmask & 0x08)) digitalWrite((pin+3), (value & 0x08));
	if ((bitmask & 0x10)) digitalWrite((pin+4), (value & 0x10));
	if ((bitmask & 0x20)) digitalWrite((pin+5), (value & 0x20));
	if ((bitmask & 0x40)) digitalWrite((pin+6), (value & 0x40));
	if ((bitmask & 0x80)) digitalWrite((pin+7), (value & 0x80));
}

void setUnusedPinsAsOutput()
{
	setPinMode(22,OUTPUT); 
	for (int i =25;i<35;i++)
		setPinMode(i,OUTPUT);
}
int getAvailableDataCountOnSerial0()
{	
	return getAvailableDataCountOnUart0();
}


int getAvailableDataCountOnSerial1()
{
	return getAvailableDataCountOnUart1();	
}

void setupMillisTimers()
{
	TCCR0=(1<<CS00)|(1<<CS01);
	SET_BIT(TIMSK,TOIE0);
}

void setupUartLeds()
{
	SET_BIT(DDRA,6);
	SET_BIT(DDRA,7);
	SET_BIT(PORTA,6);
	SET_BIT(PORTA,7);
	TCCR2|=(1<<CS20)|(1<<CS21); // clock prescalar =32
}

void sendFrameToArduino()
{
	byte dataArray[7]={0xff,0x00,0xF0,0x02,0x00,0xff,0x00};
	for (int i = 0; i < 7; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void sendArduinoToStopData()
{
	byte dataArray[10]={0xFF,0x00,0xF0,0x04,0x01,0xFE,0x01,0xFE,0x01,0x00};
	for (int i = 0; i < 10; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void sendArduinoToSendData()
{
	byte dataArray[10]={0xFF,0x00,0xF0,0x04,0x01,0xFE,0x01,0xFE,0x00,0x00};
	for (int i = 0; i < 10; i++)
	{
		writeOnUart0(dataArray[i]);
	}
}

void checkDataAvailabilityInRx0Buffer()
{
	if (getAvailableDataCountOnUart0()>0){
		dataInArduinoBuffer = true;
		storeDataInSmallBuffer = true;
	}
	else{
		dataInArduinoBuffer = false;
	}
}

void checkArduinoRx0BufferSpace()
{
	if(getIsArduinoRx0BufferOverFlowedFlag())
	{
		sendArduinoToStopData();
		setIsArduinoRx0BufferOverFlowedFlag(false);
		arduinoStopped=true;
	}
	else if (!getIsArduinoRx0BufferEmptyFlag() && !arduinoStopped)
	{
		sendArduinoToStopData();
		arduinoStopped = true;
	}
	else if(getIsArduinoRx0BufferEmptyFlag() && arduinoStopped){
		sendArduinoToSendData();
		arduinoStopped = false;
	}
}

void initialization()
{
	sei();
	setupMillisTimers();
	initFirmata();
	systemReset();
	initUart(1);
	initUart(0);
	setUnusedPinsAsOutput();
	setupUartLeds();
	requestBluetoothReset();
	sendIsAlive();
}

void catchTimeForSomeVariables()
{
	bluetoothResponseMillis=millis();
	isAliveMillis=millis();
	sentFramesMillis=millis();
}

void checkDigitalPinStatus()
{
	checkDigitalInputs();
}

void processDataFromApp()
{
	while(available()>0)
	{
		processInput();
	}
}

void checkBluetoothResetResponse()
{
	if (((newMillis-bluetoothResponseMillis)>=RESPONSE_INTERVAL) && (!bluetoothResetResponded) )
	{
		resetBluetooth();
		bluetoothResetResponded = true;
	}
}

void checkAppConnection()
{
	if (!notAliveSentToArduino)
	{
		if((newMillis-isAliveMillis)>=APP_RESPONSE_INTERVAL)
		{
			if (!isAppResponded)
			{
				sendFrameToArduino();
				notAliveSentToArduino = true;
			}
			else
			{
				sendIsAlive();
				isAliveMillis=millis();
				isAppResponded = false;
			}
		}
	}
}

void sendDataToApp()
{
	if ((newMillis-sentFramesMillis)> FRAME_GAP && (muteFirmata==0) && storeDataInSmallBuffer)
	{
		if (dataInArduinoBuffer)
		{
			if (!toggelingIndicator)
			{
				toggelingIndicator=true;
			}
			else
			{
				if(port0StatusChanged)fillBufferWithPinStates(digitalPort0array,0);
				if(port1StatusChanged)fillBufferWithPinStates(digitalPort1array,1);
				if(port2StatusChanged)fillBufferWithPinStates(digitalPort2array,2);
				toggelingIndicator= false;
			}
			
		}else{
				if(port0StatusChanged)fillBufferWithPinStates(digitalPort0array,0);
				if(port1StatusChanged)fillBufferWithPinStates(digitalPort1array,1);
				if(port2StatusChanged)fillBufferWithPinStates(digitalPort2array,2);
		}
		processUart0Input();
		for (int i=0; i<txBufferIndex; i++)
		{
			writeOnUart1(UartTx1Buffer[i]);
		}
		if(firstFrameToSend) firstFrameToSend = false;
		txBufferIndex = 0;
		storeDataInSmallBuffer=false;
		port0StatusChanged = false;
		port1StatusChanged = false;
		port2StatusChanged = false;
		sentFramesMillis=millis();
	}
}

int checkPortStateEquality(byte * oldPort ,byte * newPort,byte numberOfPins)
{
	while(--numberOfPins>0 && oldPort[numberOfPins]==newPort[numberOfPins]);
	return numberOfPins!=0;
}

void fillBufferWithPinStates(byte * portArray,byte portNumber)
{
	if(portNumber == 0)
	{
		if(checkPortStateEquality(oldDigitalPort0array,portArray,3)){
			isPort0StatusEqual = false;
			for(int i = 0 ;i <3 ; i++) oldDigitalPort0array[i]=portArray[i];
			}else{
			isPort0StatusEqual = true;
		}
		}else if(portNumber == 1){
		if(checkPortStateEquality(oldDigitalPort1array,portArray,3)){
			isPort1StatusEqual = false;
			for(int i = 0 ;i <3 ; i++) oldDigitalPort1array[i]=portArray[i];
			}else{
			isPort1StatusEqual = true;
		}
		}else if(portNumber == 2){
		if(checkPortStateEquality(oldDigitalPort2array,portArray,3)){
			isPort2StatusEqual = false;
			for(int i = 0 ;i <3 ; i++) oldDigitalPort2array[i]=portArray[i];
			}else{
			isPort2StatusEqual = true;
		}
	}
	uint8 bufferIndex = txBufferIndex;
	if (bufferIndex+3<20 && ((!isPort0StatusEqual)||(!isPort1StatusEqual)||(!isPort2StatusEqual))){
		int j = 0;
		for (int i = bufferIndex; i<bufferIndex+3 ;i++)
		{
			write(portArray[j]);
			j++;
		}
		isPort0StatusEqual = true;
		isPort1StatusEqual = true;
		isPort2StatusEqual = true;
	}
}