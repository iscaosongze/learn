#include "bist2lib.h"
#include "types.h"

void PowerOn(void);
void PowerOff(void);
LcmStru* ValidCheck(void);
void EndFlow(void);
void DispPattern(uint8 p);
void AgingFunc_WBk_100ms(unsigned char step);
void AgingFunc_WBkRGB_2s(unsigned char step);

static const uint8 CheckPattern[] = {0, 1, 2, 3, 4}; 
static const FlowStru Flow_WBk_100ms = {
        .EnterInCheckMode = NULL,
        .EndFlowSeq = EndFlow,
        .ChkList = (uint8*)CheckPattern,         
        .ChkNum = 5,
        .TotalFlow = 1,
        .Flow[0].maxSteps = 2,
        .Flow[0].loopTimes = 0x100000,
        .Flow[0].FlowFunc = AgingFunc_WBk_100ms,    
};

static const FlowStru Flow_WBkRGB_2s = {
        .EnterInCheckMode = NULL,
        .EndFlowSeq = EndFlow,
        .ChkList = (uint8*)CheckPattern,         
        .ChkNum = 5,
        .TotalFlow = 1,
        .Flow[0].maxSteps = 5,
        .Flow[0].loopTimes = 0x100000,
        .Flow[0].FlowFunc = AgingFunc_WBkRGB_2s,    
};

static const LcmStru Lcm[] = {
    {
        .Name = "LQ140M1JW46",     //Taiyuan2
        .Flow   = &Flow_WBk_100ms,  
    },{
        .Name = "LQ140M1JW49",     //Taiyuan4
        .Flow   = &Flow_WBk_100ms,  
    },{
        .Name = "LQ140M1JW47",     //Taiyuan3
        .Flow   = &Flow_WBkRGB_2s,      
    }
};

const TconStru NT71851 ={ 
    .PowerOnSeq = PowerOn,
    .PowerOffSeq = PowerOff,
    .Pattern = DispPattern,
    .ValidCheck = ValidCheck,
};

static void TCON_WriteReg(uint16 addr, uint8 p){
uint8 buf[3];
  buf[0] = (addr>>8)&0xff;
  buf[1] = (addr)&0xff;
  buf[2] = p;
  I2C_Write(0x60, buf, 3);  
}

static void TCON_WriteReg_5(uint8 p1, uint8 p2, uint8 p3, uint8 p4, uint8 p5){
    TCON_WriteReg(0x0111, p1);
    TCON_WriteReg(0x0112, p2);
    TCON_WriteReg(0x0160, p3);
    TCON_WriteReg(0x0161, p4);
    TCON_WriteReg(0x0162, p5);  
}

static LcmStru* ValidCheck(void){
uint8 tx[] = {0xFF, 0xA5, 0x0B, 0x00, 0x85, 0x71, 0x00};  
uint8 rx[11];
uint8 num;
BOOL Rslt = FALSE;
LcmStru *pLcm;

  SetGPIO(PWR_VDD, HIGH);
  _delay_ms(200);
  TCON_WriteReg(0x1376, 0x00);
  _delay_ms(1);
  Rslt = I2C_Read(0x60, tx, 7, rx, 11);
  if(Rslt == TRUE) {
      num = LcmListCount(Lcm);
      Rslt = FALSE;
      for(int ai = 0; ai < num; ai++){
          pLcm = (LcmStru*)(Lcm + ai);
          Rslt = StrCompare((char*)pLcm->Name, (char*)rx, 11);
          if(Rslt == TRUE) break;      
      }
  }
  SetGPIO(PWR_VDD, LOW);
  _delay_ms(100);
  
  if(Rslt == FALSE) return NULL;    
  return pLcm;
}
 
static void DispPattern(uint8 p){
   switch(p){
  	 case 0:  TCON_WriteReg_5(0xff, 0xf7, 0x3f, 0x3f, 0x3f);  //w
           break;
     case 1:  TCON_WriteReg_5(0x00, 0x00, 0x00, 0x00, 0x00);  //bk
           break;
     case 2:  TCON_WriteReg_5(0xf0, 0x04, 0x3f, 0x00, 0x00);  //r
           break;
     case 3:  TCON_WriteReg_5(0x0f, 0x02, 0x00, 0x3f, 0x00);  //g
           break;
     case 4:  TCON_WriteReg_5(0x00, 0xf1, 0x00, 0x00, 0x3f);  //b
           break;	
	 default: break; 
    }
}

static void PowerOn(void){
    SetGPIO(PWR_VDD, HIGH);
    _delay_ms(200);
	TCON_WriteReg(0x0004, 0xCC);
    TCON_WriteReg(0x0132, 0x00);
    TCON_WriteReg(0x020D, 0x80);
    _delay_ms(200);
    DispPattern(0);
    SetGPIO(GPO_PWM, HIGH);
    SetGPIO(GPO_BLEN, HIGH);
    _delay_ms(10);
    SetGPIO(PWR_VLED, HIGH);
    _delay_ms(100);
}

static void PowerOff(void){
    TCON_WriteReg(0x020D, 0x00);
	TCON_WriteReg(0x0111, 0xFF);
	TCON_WriteReg(0x0112, 0xF7);
	TCON_WriteReg(0x0004, 0x4C);
	TCON_WriteReg(0x0132, 0x04);
    SetGPIO(GPO_BIST, LOW);
    SetGPIO(GPO_PWM, LOW);
    SetGPIO(GPO_BLEN, LOW); 
    SetGPIO(PWR_VLED, LOW);
    _delay_ms(200);
    SetGPIO(PWR_VDD, LOW);
}

static void AgingFunc_WBk_100ms(unsigned char step){
    switch(step){
      case 0:        //W
            DispPattern(0);
            DelayL(100);
            break;
      case 1:        //BK
            DispPattern(1);
            DelayL(100);
            break;
      default: break;      
    }
}
static void AgingFunc_WBkRGB_2s(unsigned char step){
    switch(step){
      case 0:        //W
            DispPattern(0);
            DelayL(200);
            break;
      case 1:        //BK
            DispPattern(1);
            DelayL(200);
            break;
      case 2:        //R
            DispPattern(0);
            DelayL(200);
            break;
      case 3:        //G
            DispPattern(1);
            DelayL(200);
            break;  
      case 4:        //B
            DispPattern(0);
            DelayL(200);
            break;   
      default: break;      
    }
}

static void EndFlow(void){
    DispPattern(0);
}



