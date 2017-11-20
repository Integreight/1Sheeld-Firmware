#include "timers.h"
volatile uint16_t countRx=0;
volatile uint16_t countTx=0;
volatile uint16_t count=0;
volatile uint32_t timer3_millis = 0;
static	 uint8_t timer3_fract = 0;

void enableTimer3Overflow()
{
	SET_BIT(TIMSK3,TOIE3);
}
void enableTimer4Overflow()
{
	SET_BIT(TIMSK4,TOIE4);
}

void disableTimer4Overflow()
{
	CLR_BIT(TIMSK4,TOIE4);
}

void disableTimer3Overflow()
{
	CLR_BIT(TIMSK3,TOIE3);
}

void enableRxLedBlinking(){
	//isRxEnabled=1;
	countRx =1 ;
}

void enableTxLedBlinking(){
	//isTxEnabled=1;
	countTx =1 ;
}
uint32_t millis()
{
	uint32_t m;
	uint8_t oldSREG = SREG;
	// disable interrupts while we read timer3_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer3_millis)
	cli();
	m = timer3_millis;
	SREG = oldSREG;
	sei();
	return m;
}
ISR (TIMER3_OVF_vect)
{
	uint32_t m = timer3_millis;
	uint8_t f = timer3_fract;
	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
	f -= FRACT_MAX;
	m += 1;
	  }

	timer3_fract = f;
	timer3_millis = m;
}
ISR (TIMER4_OVF_vect)
{
	if(count%100==0)
	{
		if(countRx>0&&countRx<=6){
			TOG_BIT(PORTC,7);
			countRx++;
		}
		if(countTx>0&&countTx<=6){
			TOG_BIT(PORTC,6);
			countTx++;
		}
		
		if(countRx>=6&&countTx>=6)
		{
			CLR_BIT(TIMSK4,TOIE4);
			SET_BIT(PORTC,7);
			SET_BIT(PORTC,6);
		}
		else if(countRx>=6){
			//isRxEnabled=false;
			countRx=0;
			SET_BIT(PORTC,7);
		}
		else if(countTx>=6){
			//isTxEnabled=false;
			countTx=0;
			SET_BIT(PORTC,6);
		}
	}
	

	count++;
}

void setupMillisTimers()
{
	TCCR3B=(1<<CS30)|(1<<CS31);
	SET_BIT(TIMSK3,TOIE3);
}
