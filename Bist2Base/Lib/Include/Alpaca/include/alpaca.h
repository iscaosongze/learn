#ifndef __ALPACA_H__
#define __ALPACA_H__

#define  UART_MAX_TRAN_LEN   (256)
#define  UART_MAX_RECV_LEN   (1024)

#include <string.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "types.h"
#include "board.h"
#include "gpio.h"
#include "uart.h"


#include "phx_serial.h"
#include "phx_i2c.h" 

//#include "hw.h" 
//#include "hwserial.h" 
 
extern uint32_t Byte_to_Int(uint8_t *byt); 


#endif
