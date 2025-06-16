#include "alpaca.h"

void pinMode(uint8 pin, uint8 mode){
uint8 outputmode = 0;
GPIO_TypeDef *dev;

  if(pin>=BOARD_NR_GPIO_PINS)   return;
	dev = PIN_MAP[pin].gpio_dev;
	pin = PIN_MAP[pin].gpio_pin;
	switch(mode){
		case OUTPUT:
			   outputmode = (GPIO_CNFy_OUTPUT_PP | GPIO_MODEy_SPEED_50MHZ);
		     break;
		case OUTPUT_OPEN_DRAIN:
			   outputmode = (GPIO_CNFy_OUTPUT_OD | GPIO_MODEy_SPEED_50MHZ);
		     break;	
    case INPUT:
         outputmode = (GPIO_CNFy_INPUT_FLOAT);
         break;	
    case INPUT_PULLDOWN:
			   outputmode = (GPIO_CNFy_INPUT_PD);
		     dev->ODR = dev->ODR & (~(1<<pin));
         break;
    case INPUT_PULLUP:
			   outputmode = (GPIO_CNFy_INPUT_PU);
		     dev->ODR = dev->ODR | (1<<pin);
         break;			
		default: break;	
	}	
    __IO uint32 *cr = &dev->CRL + (pin >> 3);
    uint32 shift = (pin & 0x7) * 4;
    uint32 tmp = *cr;

    tmp &= ~(0xF << shift);
    tmp |= (outputmode << shift);
    *cr = tmp;		
}

void digitalWrite(uint8 pin, uint8 val){
	 if(pin>=BOARD_NR_GPIO_PINS)   return;
	 val = !val;          /* "set" bits are lower than "reset" bits  */  
	 PIN_MAP[pin].gpio_dev->BSRR = (1U << PIN_MAP[pin].gpio_pin) << (16 * val);
}	

uint32 digitalRead(uint8 pin) {
    if (pin >= BOARD_NR_GPIO_PINS) {
        return 0;
    }

    if(PIN_MAP[pin].gpio_dev->IDR & (1U<<PIN_MAP[pin].gpio_pin)) return HIGH;
		return LOW;
}

void digitalToggle(uint8 pin) {
    if (pin >= BOARD_NR_GPIO_PINS) {
        return;
    }
    PIN_MAP[pin].gpio_dev->ODR ^= (1u << PIN_MAP[pin].gpio_pin); 
}

void multiPinMode(uint8 *pins, uint8 len, uint8 mode){
	  for(int ai = 0; ai<len; ai++){
				pinMode(pins[ai], mode);
		}	
}

uint32 digitalGet(uint8 *pins, uint8 len){
uint32 val = 0;
    for(int ai = len; ai>0; ai--){
        val = (val<<1) + digitalRead(pins[ai-1]);
    }	
    return val;		
}	
