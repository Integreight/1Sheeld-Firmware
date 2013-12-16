#ifndef Mapping_h
#define Mapping_h

#ifndef digitalPinHasPWM
#define digitalPinHasPWM(p)     IS_PIN_DIGITAL(p)
#endif



#define TOTAL_PINS              35 
#define IS_PIN_DIGITAL(p)       ((p) >= 2 && (p) <= 22) || ((p)>=25 && (p)<=34)  
#define IS_PIN_PWM(p)           digitalPinHasPWM(p)
#define IS_PIN_SPI(p)           ((p) == SS || (p) == MOSI || (p) == MISO || (p) == SCK)
#define PIN_TO_DIGITAL(p)       (p)
#define PIN_TO_PWM(p)           PIN_TO_DIGITAL(p)
#define PINOUT_OPTIMIZE         1






#endif 
