#include "bist2lib.h"
#include "types.h"

 
void PowerOn(void);
void PowerOff(void);
LcmStru* ValidCheck(void);
void EndFlow(void);
void DispPattern(uint8 p);
void AgingFunc_A(unsigned char step);

static const uint8 CheckPattern[] = {0, 1, 2, 3, 4, 5}; 
static const FlowStru Flow_0 = {
        .EnterInCheckMode = NULL,
        .EndFlowSeq = EndFlow,
        .ChkList = (uint8*)CheckPattern,         
        .ChkNum = 6,
        .TotalFlow = 1,
        .Flow[0].maxSteps = 2,
        .Flow[0].loopTimes = 0x100000,
        .Flow[0].FlowFunc = AgingFunc_A,    
};

static const LcmStru Lcm[] = {
    {
        .Name = "LQ134R1JX44",     //Santos2
        .IdAddr = 0x7FE0,
        .Flow   = &Flow_0,  
    },{
        .Name = "LQ134R1JX45",     //Seattle2
        .IdAddr = 0x7FE0,
        .Flow   = &Flow_0,      
    },{
        .Name = "LQ134N1JW45",     //Rio2
        .IdAddr = 0x3FE0,
        .Flow   = &Flow_0,
    },{
        .Name = "LQ134N1JW50",     //Rio3
        .IdAddr = 0x3FE0,
        .Flow   = &Flow_0,       
    },    
};

const TconStru ANX2404 ={ 
    .PowerOnSeq = PowerOn,
    .PowerOffSeq = PowerOff,
    .Pattern = DispPattern,
    .ValidCheck = ValidCheck,
}; 

static void TCON_WriteReg(uint8 osb, uint8 addr1, uint8 addr2, uint8 addr3, uint8 p){
uint8 buf[5];
  buf[0] = 0x00;
  buf[1] = osb;
  buf[2] = addr1;
  buf[3] = addr2;
  buf[4] = addr3;  
  I2C_Write(0x3E, buf, 5); 

  buf[0] = 0x00;
  buf[1] = p; 
  I2C_Write(0x44, buf, 2); 
  
}
 

static LcmStru* ValidCheck(void){
uint8 tx[2];  
uint8 rx[11];
uint8 ai, num;
BOOL Rslt = FALSE;
LcmStru *pLcm;
    SetGPIO(PWR_VDD, HIGH);
    _delay_ms(800);
    
    num = LcmListCount(Lcm);
    for(ai = 0; ai <num; ai++){
         pLcm = (LcmStru*)(Lcm + ai);
         tx[0] = (pLcm->IdAddr>>8)&0xff; 
         tx[1] = pLcm->IdAddr &0xff;
         Rslt = I2C_Read(0x50, tx, 2, rx, 11);  
         if(Rslt == TRUE) {
             Rslt = FALSE;
             for(int bi = 0; bi < num; bi++){        
                 Rslt = StrCompare((char*)pLcm->Name, (char*)rx, 11);
                 if(Rslt == TRUE) break;
             }
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
  	 case 0:  TCON_WriteReg(0x0b, 0x00, 0x00, 0x92, 0x02);  //w
              TCON_WriteReg(0x0b, 0x00, 0x01, 0x99, 0x0d);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x01, 0x20);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x02, 0x00);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x03, 0x00);     
           break;
  	 case 1:  TCON_WriteReg(0x0b, 0x00, 0x00, 0x92, 0x02);  //V127
              TCON_WriteReg(0x0b, 0x00, 0x01, 0x99, 0x0d);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x01, 0x00);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x02, 0x00);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x03, 0x02);  
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x1B, 0xFF); 
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x1C, 0xFF);  
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x1D, 0xFF);  
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x55, 0x7F);   
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x56, 0x00);   
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x7A, 0x00);      
           break;
  	 case 2:  TCON_WriteReg(0x0b, 0x00, 0x00, 0x92, 0x02);  //bk
              TCON_WriteReg(0x0b, 0x00, 0x01, 0x99, 0x0d);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x01, 0x02);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x02, 0x00);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x03, 0x00);     
           break;
  	 case 3:  TCON_WriteReg(0x0b, 0x00, 0x00, 0x92, 0x02);  //r
              TCON_WriteReg(0x0b, 0x00, 0x01, 0x99, 0x0d);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x01, 0x00);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x02, 0x20);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x03, 0x00);     
           break;
  	 case 4:  TCON_WriteReg(0x0b, 0x00, 0x00, 0x92, 0x02);  //g
              TCON_WriteReg(0x0b, 0x00, 0x01, 0x99, 0x0d);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x01, 0x00);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x02, 0x02);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x03, 0x00);     
           break;
  	 case 5:  TCON_WriteReg(0x0b, 0x00, 0x00, 0x92, 0x02);  //b
              TCON_WriteReg(0x0b, 0x00, 0x01, 0x99, 0x0d);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x01, 0x00);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x02, 0x00);
              TCON_WriteReg(0x0b, 0x00, 0x00, 0x03, 0x20);     
           break;
    }
}

static void PowerOn(void){
    SetGPIO(PWR_VDD, HIGH);
	
    _delay_ms(1000);
      DispPattern(0);
    TCON_WriteReg(0x00, 0x00, 0x01, 0x0a, 0x80);	
	  _delay_ms(10);

    SetGPIO(PWR_VLED, HIGH);	
	  _delay_ms(10);
    SetGPIO(GPO_PWM, HIGH);
		_delay_ms(10);
    SetGPIO(GPO_BLEN, HIGH); 
    _delay_ms(50);
	       
}

static void PowerOff(void){
    TCON_WriteReg(0x00, 0x00, 0x01, 0x0a, 0x00);
    SetGPIO(GPO_BIST, LOW);
    SetGPIO(GPO_PWM, LOW);
    SetGPIO(GPO_BLEN, LOW); 
    SetGPIO(PWR_VLED, LOW);
    _delay_ms(200);
    SetGPIO(PWR_VDD, LOW);
	SetGPIO(GPO_BIST, LOW);
}

static void AgingFunc_A(unsigned char step){
    switch(step){
      case 0:        //W
            DispPattern(0);
            DelayL(100);
            break;
      case 1:        //BK
            DispPattern(2);
            DelayL(100);
            break;
      default: break;      
    }
}

static void EndFlow(void){
    DispPattern(0);
}



