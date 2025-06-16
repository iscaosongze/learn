#ifndef  _BOARD_GENERIC_STM32F103C8_H_
#define  _BOARD_GENERIC_STM32F103C8_H_

typedef struct stm32_pin_info{
	GPIO_TypeDef *gpio_dev;
	uint8 gpio_pin;
}stm32_pin_info;

#define BOARD_NR_GPIO_PINS        35


enum {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13,PA14,PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13,PB14,PB15,
	PC13, PC14,PC15
};

extern const stm32_pin_info PIN_MAP[];


#endif
