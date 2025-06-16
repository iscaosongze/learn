#include "alpaca.h"
#include "stdio.h"

USART_TypeDef *UartGetBaseAddress(uint8 ch){
USART_TypeDef  *uart;   
    switch(ch){ 
        case  1:   uart = USART1; break;   
        case  2:   uart = USART2; break;   
        case  3:   uart = USART3; break;   
        case  4:   uart = UART4; break;   
        case  5:   uart = UART5; break; 
        case  6:   uart = USART6; break; 
        default: uart = 0;  break;
    }
    return uart;
}

uint8 UartWaitFinish(uint8 ch){
USART_TypeDef  *uart;    
    uart = UartGetBaseAddress(ch);
    if(uart == 0) return FALSE;
    while((uart->SR & USART_SR_TC) == 0); //TODO: add timeout   
    return TRUE;    
}

uint8 UartPutByte(uint8 ch, uint8 byt){
USART_TypeDef  *uart;
    uart = UartGetBaseAddress(ch);
    if(uart == 0) return 0;    
    uart->DR = byt;
    while((uart->SR & USART_SR_TXE) == 0); //TODO: add timeout   
    return 1;    
}    
uint8 UartGetByte(uint8 ch){
USART_TypeDef  *uart;
uint8 byt;    
    uart = UartGetBaseAddress(ch);
    if(uart == 0) return 0;    
    byt = uart->DR & 0x00FF; 
    return byt;    
}

uint8 UartEnable(uint8 ch, BOOL RxEnable, BOOL TxEnable){
USART_TypeDef  *uart;
    uart = UartGetBaseAddress(ch);
    if(uart == 0) return 0;      
  if (RxEnable) {   
    SET_BIT(uart->CR1, UART_IT_RXNE);    
  } else {    
    CLEAR_BIT(uart->CR1, UART_IT_RXNE);
  }
	//TODO: need fixed
  return TRUE;
}



