#include "alpaca.h"
#include "phx_i2c.h"

#define nop()                  __NOP()

#define I2C_SDA_H(_this)        pinMode(_this->SdaPort, INPUT) 
#define I2C_SDA_L(_this)        pinMode(_this->SdaPort, OUTPUT_OPEN_DRAIN)
#define I2C_SDA(_this)          digitalRead(_this->SdaPort)
#define I2C_SCL_H(_this)        pinMode(_this->SclPort, INPUT) 
#define I2C_SCL_L(_this)        pinMode(_this->SclPort, OUTPUT_OPEN_DRAIN)  
#define I2C_SCL(_this)          digitalRead(_this->SclPort)
 
#define I2C_OK                  TRUE 
#define I2C_ERR                 FALSE

static void delay(void){
    for(int ai = 0; ai< 80; ai++) 
      __NOP();
}

void PHX_I2C_Initial(I2CStru *_this, uint8 sclPort, uint8 sdaPort){
    _this->SclPort = sclPort;
    _this->SdaPort = sdaPort;
    digitalWrite(_this->SclPort, LOW);
    digitalWrite(_this->SdaPort, LOW);
    I2C_SCL_H(_this);
    I2C_SDA_H(_this);
}

void I2C_Start(I2CStru *_this){
	I2C_SDA_H(_this); nop();
	I2C_SCL_H(_this);
	delay(); 
	I2C_SDA_L(_this);
	delay(); 
	I2C_SCL_L(_this);
	delay(); 
}

void I2C_Stop(I2CStru *_this)	{
	I2C_SDA_L(_this);  nop();
	I2C_SCL_L(_this);	nop();
	I2C_SCL_H(_this);
	delay(); delay();
	I2C_SDA_H(_this);
	delay(); delay();
}
BOOL I2C_WaitForSCLHigh(I2CStru *_this){
uint8 dy = 0xff;
	while(I2C_SCL(_this) == 0){
		dy--;
		if(dy == 0)
		   return I2C_ERR;
	}
	return I2C_OK;
}

uint8 I2C_GetAck(I2CStru *_this){
uint8 ack;
	I2C_SDA_H(_this);
 	nop();
    I2C_SCL_H(_this);
    if(I2C_WaitForSCLHigh(_this) != I2C_OK)
	     return 0;
   delay();		 
	if(I2C_SDA(_this) == 0)
	     ack = 0;
	else
	     ack = 1;	 
	 I2C_SCL_L(_this);
   delay();
  
	return ack;	 
}

void I2C_SendACK(I2CStru *_this, uint8 ack){
	if(ack)
	   I2C_SDA_H(_this);
    else
	    I2C_SDA_L(_this);
  delay();
	I2C_SCL_H(_this);
    if(I2C_WaitForSCLHigh(_this) != I2C_OK){
	   I2C_Stop(_this);
       return ;
	}
	delay();
	I2C_SCL_L(_this);
	delay();
    I2C_SDA_H(_this);
  
}
 
uint8 PHX_I2C_SendByte(I2CStru *_this, uint8 d){
uint8 BitCount;
uint8 ack;

	ack=0;
	for(BitCount = 0; BitCount < 8; BitCount ++) {   
		if(d&0x80) I2C_SDA_H(_this);  else  I2C_SDA_L(_this);
		d = d<<1;	
    delay();     
		I2C_SCL_H(_this);
    delay(); delay();      
		I2C_SCL_L(_this);
    delay();      			
	 }
	ack = I2C_GetAck(_this);
	return ack; 
}

uint8 PHX_I2C_ReadByte(I2CStru *_this){
 unsigned char bitSelect;
 unsigned char rst = 0;
  
    rst = 0;
	  bitSelect = 0x80;
    I2C_SDA_H(_this);
	while(bitSelect){
		I2C_SCL_H(_this);
		I2C_WaitForSCLHigh(_this);
    delay();
		if(I2C_SDA(_this))
		   rst |= bitSelect;
		nop();	
		bitSelect >>= 1;
		I2C_SCL_L(_this);
		delay(); 	
  }
	return rst;
}

uint8 PHX_I2C_Write(I2CStru *_this, uint8 slvAddr, uint8 *tx, uint16 txLen){
unsigned char i;
uint8 Rslt = FALSE;    
	I2C_Start(_this);
    Rslt =PHX_I2C_SendByte(_this, slvAddr<<1);
	for(i=0; i<txLen; i++){
		Rslt = PHX_I2C_SendByte(_this, tx[i]);
	}
  I2C_Stop(_this);
  return !Rslt;  
}

uint8 PHX_I2C_Read(I2CStru *_this, uint8 slvAddr, uint8 *tx, uint16 txLen, uint8 *rx, uint16 rxLen){
uint8 Rslt = FALSE;     
  I2C_Start(_this);
  Rslt = PHX_I2C_SendByte(_this, slvAddr<<1);  
  for(int ai = 0; ai<txLen; ai++)
      Rslt = PHX_I2C_SendByte(_this, tx[ai]);
  I2C_Start(_this);
  Rslt =PHX_I2C_SendByte(_this, (slvAddr<<1)+1);
  for(int ai = 0; ai<rxLen; ai++){
      rx[ai] = PHX_I2C_ReadByte(_this);
      if(ai == rxLen - 1)
          I2C_SendACK(_this, 1);
      else
          I2C_SendACK(_this, 0);        
  }    
  I2C_Stop(_this);
  return !Rslt;
}

