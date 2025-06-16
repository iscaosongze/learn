#include "alpaca.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "def.h"
#include "stateled.h"


void Lcm_Add(LcmStru *lcm){
   //AGB->TconList[AGB->TconNum++] = lcm;
}  
void Tcon_Add(TconStru *tcon){
   AGB->TconList[AGB->TconNum++] = tcon;
}

void Lcm_Create(void){
  
}

uint8 I2C_Write(uint8 slvAddr, uint8 *buf, uint16 size){
AgingStru *aging = (AgingStru*)xTaskGetApplicationTaskTag(NULL);
HwStru *hw = aging->Hw;    
uint8 Rslt = FALSE;     
    taskENTER_CRITICAL();
    Rslt = PHX_I2C_Write(hw->I2cDev, slvAddr, buf, size);
    taskEXIT_CRITICAL();
    return Rslt;
}  

uint8 I2C_Read(uint8 slvAddr, uint8 *txbuf, uint16 txLen, uint8 *rxbuf, uint16 rxLen){
AgingStru *aging = (AgingStru*)xTaskGetApplicationTaskTag(NULL);
HwStru *hw = aging->Hw;  
uint8 Rslt = FALSE;     
    taskENTER_CRITICAL();  
    Rslt = PHX_I2C_Read(hw->I2cDev, slvAddr, txbuf, txLen, rxbuf, rxLen);
    taskEXIT_CRITICAL();
    return Rslt;    
}

void SetGPIO(uint8 gpo, uint8 val){
AgingStru *aging = (AgingStru*)xTaskGetApplicationTaskTag(NULL);
HwStru *hw = aging->Hw;  
  switch(gpo){
    case PWR_VDD:
         digitalWrite(hw->VddPort, val);
         break;       
    case PWR_VLED:
         digitalWrite(hw->VledPort, val);
         break;       
    case GPO_PWM:
         digitalWrite(hw->PwmPort, val);
         if(aging->Ch==0)
             digitalWrite(PD14, val);
         break;       
    case GPO_BLEN:
         digitalWrite(hw->BlenPort, val);
         break;       
    case GPO_BIST:
         digitalWrite(hw->BistPort, val);
         break; 
    case GPO_WP:
         digitalWrite(hw->WpPort, val);
         break;
    case GPO_LED:
         if(val == 0){
              pinMode(hw->LedPort, INPUT);
         }else if(val == 1){
              digitalWrite(hw->LedPort, HIGH);
              pinMode(hw->LedPort, OUTPUT);
         }else if(val == 2){
              digitalWrite(hw->LedPort, LOW);
              pinMode(hw->LedPort, OUTPUT);         
         }           
         break;  
    case GPO_VTP:
         digitalWrite(hw->VtpPort, val);
         break;    
  }
}

uint8 GetGPI(uint8 gpi){
uint8 val = 0;    
    switch(gpi){
       case GPI_0:
          val = digitalRead(GPI0_PIN);
          break; 
    }
    return val; 
}

void _delay_ms(uint32 t){
   vTaskDelay(t);
}  

void DelayL(uint32 t){
AgingStru *aging = (AgingStru*)xTaskGetApplicationTaskTag(NULL);  
    //aging->m_loopDelay = HAL_GetTick() + t;
    aging->m_loopDelay = xTaskGetTickCount() + t;
}

BOOL StrCompare(char *p1, char *p2, uint16 len){
BOOL Rslt = FALSE; 
int ai;    
    if(len != 0){
        for(ai = 0; ai<len; ai++){
             if(p1[ai] != p2[ai])  break;
      }
      if(ai == len)
          Rslt = TRUE;      
    }
    return Rslt;
}
