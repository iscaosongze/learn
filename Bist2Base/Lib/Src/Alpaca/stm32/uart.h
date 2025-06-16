#ifndef __STM32_UART_H
#define __STM32_UART_H

#ifdef __cplusplus
extern "C" {
#endif
uint8 UartEnable(uint8 ch, BOOL RxEnable, BOOL TxEnable);
uint8 UartPutByte(uint8 ch, uint8 byt);
uint8 UartGetByte(uint8 ch);
uint8 UartWaitFinish(uint8 ch);    
    
#ifdef __cplusplus
}
#endif

#endif
