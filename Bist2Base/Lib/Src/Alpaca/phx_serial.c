#include "alpaca.h"
#include "stdio.h"
 
void PHX_SerialInitial(SerialStru *_this, EventFunc func){
    _this->Event = (void*)func;
    pinMode(_this->RS485EnablePort, OUTPUT);
    UartEnable(_this->Uart, TRUE, FALSE);
    digitalWrite(_this->RS485EnablePort, LOW);
}

BOOL PHX_SerialGetByte(SerialStru *uart, uint8 *byt){ 
    *byt = UartGetByte(uart->Uart);
  return TRUE;
}	

BOOL PHX_SerialPutByte(SerialStru *uart, uint8 byt){
	UartPutByte(uart->Uart, byt);
    return TRUE;
}

BOOL PHX_Serial_Transmit(SerialStru *uart, uint8 *buf, uint16 len){

    for(int ai=0; ai<len; ai++){
        UartPutByte(uart->Uart, buf[ai]);
    } 
    if(uart->bRS485 == 1){
        UartWaitFinish(uart->Uart);
 //       digitalWrite(uart->RS485EnablePort, LOW);
    }    
    return TRUE;    
}

void PHX_SerialWriteP(SerialStru *_this, uint8 *buf, uint16 len){
uint8 tx[256];
    if(_this->bRS485 == 1){
        digitalWrite(_this->RS485EnablePort, HIGH);
        for(int ai = 0; ai<2000; ai++) __NOP();
    }      
	PHX_SerialPutByte(_this, 0x02);
  PHX_SerialPutByte(_this, (len&0xff00)>>8);
	PHX_SerialPutByte(_this, len&0xff);
	if(len != 0){
      PHX_Serial_Transmit(_this, buf, len);
 	}
  PHX_SerialPutByte(_this, 0x03);	
  if(_this->bRS485 == 1){
      UartWaitFinish(_this->Uart);
      digitalWrite(_this->RS485EnablePort, LOW);
  }
}

void CSZ_SerialWriteP(SerialStru *_this, uint8 cmd, uint8 *buf, uint16 len){
uint8 tx[256];
    if(_this->bRS485 == 1){
        digitalWrite(_this->RS485EnablePort, HIGH);
        for(int ai = 0; ai<2000; ai++) __NOP();
    }      
	PHX_SerialPutByte(_this, 0x02);
  PHX_SerialPutByte(_this, (len&0xff00)>>8);
	PHX_SerialPutByte(_this, len&0xff);
	PHX_SerialPutByte(_this, cmd);
	if(len != 0){
      PHX_Serial_Transmit(_this, buf, len-1);
 	}
  PHX_SerialPutByte(_this, 0x03);	
  if(_this->bRS485 == 1){
      UartWaitFinish(_this->Uart);
      digitalWrite(_this->RS485EnablePort, LOW);
  }
}

/* 串口中断事件函数 */
void PHX_SerialEvent(SerialStru *_this){  
    if(_this->bNewEvent){
         if(_this->Event !=0)
              _this->Event(_this);
         _this->bNewEvent = 0;  
    }      
}  

void PHX_SerialReceiveISR(SerialStru *uart){
uint8 ch;
uint32 curTick;	
	  PHX_SerialGetByte(uart, &ch); 
	
    curTick= HAL_GetTick();	
    if(curTick > uart->TimeoutCnt+5){
         uart->State = UART_IDLE;
    }	
	  uart->TimeoutCnt = curTick;
 
    switch(uart->State){
      case UART_IDLE: 
            if(ch == 0x02)		 
              uart->State = UART_LEN0;
           break;
      case UART_LEN0:
             uart->RecvLen = ch;
             uart->State = UART_LEN1;
           break;
      case UART_LEN1:
              uart->RecvLen = uart->RecvLen * 256 + ch;
              uart->nData = 0;
              if(uart->RecvLen==0)
                 uart->State = UART_END;
              else
                uart->State = UART_DATA;
           break;
      case UART_DATA:
              uart->RecvBuf[uart->nData++] = ch;
              if(uart->nData == uart->RecvLen)
                uart->State = UART_END;			
           break;
      case UART_END:
              uart->State = UART_IDLE;
              if(ch == 0x03){
                  uart->bNewEvent = 1;
              }else
                  uart->bNewEvent = 0;
           break;
      default: break;			
    }	  
}  

