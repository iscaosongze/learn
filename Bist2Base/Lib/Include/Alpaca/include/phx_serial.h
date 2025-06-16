#ifndef __PHX_SERIAL_H__
#define __PHX_SERIAL_H__


#ifdef __cplusplus
extern "C" {
#endif

enum {
    UART_IDLE = 0x00,
    UART_HEAD, 
	  UART_ADDR,
    UART_LEN0, 
    UART_LEN1, 
    UART_DATA, 
    UART_END,
	  UART_CMD,
};
  
 
typedef struct{
   uint8                    Uart;               /* */
   uint8                    bRS485; 
   uint8                    RS485EnablePort;
  
   uint8                    State;         /*串口接收程序状态机 */
   uint8                    bNewEvent;
   uint32                   TimeoutCnt;   
	 uint8                    RecvBuf[UART_MAX_RECV_LEN];
	 uint16                   RecvLen;	
	 uint16                   nData; 
   void                     (*Event)(void *_this);  
}SerialStru;
    
typedef void (*EventFunc)(SerialStru *uart);
void PHX_SerialInitial(SerialStru *_this, EventFunc func);
BOOL PHX_SerialGetByte(SerialStru *uart, uint8 *byt);
BOOL PHX_SerialPutByte(SerialStru *uart, uint8 byt);
BOOL PHX_Serial_Transmit(SerialStru *uart, uint8 *buf, uint16 len);
void PHX_SerialWriteP(SerialStru *_this, uint8 *buf, uint16 len);
void CSZ_SerialWriteP(SerialStru *_this, uint8 cmd, uint8 *buf, uint16 len);
void PHX_SerialEvent(SerialStru *uart);
void PHX_SerialReceiveISR(SerialStru *uart);

#ifdef __cplusplus
}
#endif

#endif
