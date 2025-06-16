#include "alpaca.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "bist2lib.h"
#include "board.h"
#include "StateLed.h"
#include "def.h"


LedStEnum StateLed[AGING_CH_MAX];


void Led_Task(void * argument){ 
uint32 LedCnt[AGING_CH_MAX]; 
  uint8  led[AGING_CH_MAX];   /* 0: On, 1: Off */
   while(1){
      for(int ai = 0; ai< AGING_CH_MAX; ai++){
          AgingStru *ag = AGB->Aging[ai];
          switch(ag->LedState){
            case LED_ALL_OFF:
                    LedCnt[ai] = 0;
                    led[ai] = 0;
                    pinMode(ag->Hw->LedPort, INPUT);
                  break;
            case GLED_ON: 
                    LedCnt[ai] = 0;
                    led[ai] = 1;
                    digitalWrite(ag->Hw->LedPort, HIGH);
                    pinMode(ag->Hw->LedPort, OUTPUT);              
                  break;              
            case RLED_ON:
                    LedCnt[ai] = 0; 
                    led[ai] = 2;            
                    digitalWrite(ag->Hw->LedPort, LOW);
                    pinMode(ag->Hw->LedPort, OUTPUT);                
                  break;              
            case GLED_SLOW_FLK:
                    if(LedCnt[ai] == 0){
                        LedCnt[ai] = xTaskGetTickCount() + 800;
                    }else if(xTaskGetTickCount() > LedCnt[ai]){
                        LedCnt[ai] = xTaskGetTickCount() + 800;
                        if(led[ai] == 1){
                            led[ai] = 0;
                            pinMode(ag->Hw->LedPort, INPUT); 
                        }else{
                            led[ai] = 1;
                            digitalWrite(ag->Hw->LedPort, HIGH);
                            pinMode(ag->Hw->LedPort, OUTPUT); 
                        }                                                        
                    }                      
                  break;              
            case GLED_QUICK_FLK: 
                    if(LedCnt[ai] == 0){
                        LedCnt[ai] = xTaskGetTickCount() + 300;
                    }else if(xTaskGetTickCount() > LedCnt[ai]){
                        LedCnt[ai] = xTaskGetTickCount() + 300;
                        if(led[ai] == 1){
                            led[ai] = 0;
                            pinMode(ag->Hw->LedPort, INPUT);
                        }else{
                            led[ai] = 1;
                            digitalWrite(ag->Hw->LedPort, HIGH);
                            pinMode(ag->Hw->LedPort, OUTPUT); 
                        }                                                        
                    }                 
                  break;              
            case RLED_SLOW_FLK:
                    if(LedCnt[ai] == 0){
                        LedCnt[ai] = xTaskGetTickCount() + 800;
                    }else if(xTaskGetTickCount() > LedCnt[ai]){
                        LedCnt[ai] = xTaskGetTickCount() + 800;
                        if(led[ai] == 2){
                            led[ai] = 0;
                            pinMode(ag->Hw->LedPort, INPUT); 
                        }else{
                            led[ai] = 2;
                            digitalWrite(ag->Hw->LedPort, LOW);
                            pinMode(ag->Hw->LedPort, OUTPUT); 
                        }                                                        
                    }                 
                  break;              
            case RLED_QUICK_FLK:
                    if(LedCnt[ai] == 0){
                        LedCnt[ai] = xTaskGetTickCount() + 200;
                    }else if(xTaskGetTickCount() > LedCnt[ai]){
                        LedCnt[ai] = xTaskGetTickCount() + 200;
                        if(led[ai] == 2){
                            led[ai] = 0;
                            pinMode(ag->Hw->LedPort, INPUT); 
                        }else{
                            led[ai] = 2;
                            digitalWrite(ag->Hw->LedPort, LOW);
                            pinMode(ag->Hw->LedPort, OUTPUT); 
                        }                                                        
                    }                 
                 break;              
          }
      }
      vTaskDelay(1);
   }
}

