#include "alpaca.h"
/* STM32F407 
 *
 */
void pinMode(uint8 pin, uint8 mode){
GPIO_TypeDef *dev;
uint8 pinx2;
uint32 mod;  
  if(pin>=BOARD_NR_GPIO_PINS)   return;
	dev = PIN_MAP[pin].gpio_dev;
	pin = PIN_MAP[pin].gpio_pin;
    pinx2 = pin<<1;
	switch(mode){
		case OUTPUT:
               mod = dev->MODER;
               mod &= ~(3<<pinx2); mod |= (1<<pinx2);  
               dev->MODER = mod;               
               //dev->MODER &= ~(3<<pinx2); dev->MODER |= (1<<pinx2); 
               dev->OSPEEDR &= ~(3<<pinx2);  dev->OSPEEDR |= (1<<pinx2); //Medium speed 
               dev->OTYPER &=~(1<<pin);     //Push-pull
		     break;
		case OUTPUT_OPEN_DRAIN:
               mod = dev->MODER;
               mod &= ~(3<<pinx2); mod |= (1<<pinx2);  
               dev->MODER = mod;
               //dev->MODER &= ~(3<<pinx2); dev->MODER |= (1<<pinx2); 
               dev->OSPEEDR &= ~(3<<pinx2);  dev->OSPEEDR |= (1<<pinx2); //Medium speed 
               dev->OTYPER |= 1<<pin ;     //Open-drain            
		     break;	
        case INPUT:
               mod = dev->MODER;
               mod &= ~(3<<pinx2);   
               dev->MODER = mod;          
               //dev->MODER &= ~(3<<pinx2);  
               dev->OSPEEDR &= ~(3<<pinx2);  dev->OSPEEDR |= (1<<pinx2); //Medium speed 
               dev->OTYPER |= 1<<pin ;     //Open-drain   
         break;	
    case INPUT_PULLDOWN:
               mod = dev->MODER;
               mod &= ~(3<<pinx2);   
               dev->MODER = mod;      
               //dev->MODER &= ~(3<<pinx2);  
               dev->OSPEEDR &= ~(3<<pinx2);  dev->OSPEEDR |= (1<<pinx2); //Medium speed 
               dev->PUPDR &= ~(3<<pinx2);  dev->PUPDR |= (2<<pinx2);       //pull-down
         break;
    case INPUT_PULLUP:
               mod = dev->MODER;
               mod &= ~(3<<pinx2);   
               dev->MODER = mod;      
               //dev->MODER &= ~(3<<pinx2);  
               dev->OSPEEDR &= ~(3<<pinx2);  dev->OSPEEDR |= (1<<pinx2); //Medium speed 
               dev->PUPDR &= ~(3<<pinx2);  dev->PUPDR |= (1<<pinx2);       //pull-UP
         break;			
		default: break;	
	}			
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
