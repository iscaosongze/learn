#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO_CNFy_INPUT_ANALOG               (0 << 2)
#define GPIO_CNFy_INPUT_FLOAT                (1 << 2)
#define GPIO_CNFy_INPUT_PU                   (2 << 2)
#define GPIO_CNFy_INPUT_PD                   (2 << 2)
#define GPIO_CNFy_INPUT_OD                   (3 << 2)

#define GPIO_CNFy_OUTPUT_PP                  (0 << 2)
#define GPIO_CNFy_OUTPUT_OD                  (1 << 2)
#define GPIO_CNFy_OUTPUT_AF_PP               (2 << 2)
#define GPIO_CNFy_OUTPUT_AF_OD               (3 << 2)


#define GPIO_MODEy_SPEED_10MHZ               (1 << 0)
#define GPIO_MODEy_SPEED_2MHZ                (2 << 0)
#define GPIO_MODEy_SPEED_50MHZ               (3 << 0) 


void pinMode(uint8 pin, uint8 mode);
void digitalWrite(uint8 pin, uint8 val);
uint32 digitalRead(uint8 pin);
void digitalToggle(uint8 pin);
void multiPinMode(uint8 *pins, uint8 len, uint8 mode);
uint32 digitalGet(uint8 *pins, uint8 len);

#endif
