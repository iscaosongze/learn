#ifndef __PHX_I2C_H__
#define __PHX_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif
typedef struct{
   uint8                    Ch;               /* */
   uint8                    SclPort; 
   uint8                    SdaPort;
   uint8                    SlvAddr;
   
   uint8                    State;         /*串口接收程序状态机 */
   uint8                    bNewEvent;
   uint32                   TimeoutCnt;   
	 uint8                    RecvBuf[UART_MAX_RECV_LEN];
	 uint16                   RecvLen;	
	 uint16                   nData; 
   void                     (*Event)(void *_this);  
}I2CStru;

extern void PHX_I2C_Initial(I2CStru *_this, uint8 sclPort, uint8 sdaPort);    
extern unsigned char  PHX_I2C_SendByte(I2CStru *_this, unsigned char d);
extern unsigned char PHX_I2C_ReadByte(I2CStru *_this);
extern uint8 PHX_I2C_Write(I2CStru *_this, uint8 slvAddr, uint8 *dat, uint16 len);    
extern uint8 PHX_I2C_Read(I2CStru *_this, uint8 slvAddr, uint8 *tx, uint16 txLen, uint8 *rx, uint16 rxLen);

#ifdef __cplusplus
}
#endif

#endif
