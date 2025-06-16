#include "alpaca.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bist2lib.h"

#include "def.h" 
#include "stateled.h"
#include "PHX_Flash.h"

uint8    rmtKeyCode; 
uint8 DATA_tmp[1023];	


TaskHandle_t AgingTask_Handle[8];
void Aging_Task(void * argument); 
TaskHandle_t SysTask_Handle;
void Sys_Task(void * argument); 
TaskHandle_t LedTask_Handle;
void Led_Task(void * argument); 

void KeyFunc(AgingStru *_this);
void AgingFunc(AgingStru *_this);
extern unsigned int Version[];
extern HwStru _Hw[];
/************************************************
 *
 * Task Create
 *
 ************************************************
 */
/* 线程创建 */
BaseType_t Task_Create(void){
BaseType_t xReturn = pdPASS;
 /* 显示版本 */
    uint8 ver = Version[0] &0x0FF;
    ver = (ver>>4)*10 + (ver&0x0F); 
    
    if(ver&0x80){
        digitalWrite(_Hw[0].LedPort, HIGH);
        pinMode(_Hw[0].LedPort, OUTPUT);
        digitalWrite(_Hw[4].LedPort, HIGH);
        pinMode(_Hw[4].LedPort, OUTPUT);        
    }else{
        pinMode(_Hw[0].LedPort, INPUT);
        pinMode(_Hw[4].LedPort, INPUT);
    }
    if(ver&0x40){
        digitalWrite(_Hw[1].LedPort, HIGH);
        pinMode(_Hw[1].LedPort, OUTPUT);
        digitalWrite(_Hw[5].LedPort, HIGH);
        pinMode(_Hw[5].LedPort, OUTPUT);        
    }else{
        pinMode(_Hw[1].LedPort, INPUT);
        pinMode(_Hw[5].LedPort, INPUT);
    } 
    if(ver&0x20){
        digitalWrite(_Hw[2].LedPort, HIGH);
        pinMode(_Hw[2].LedPort, OUTPUT);
        digitalWrite(_Hw[6].LedPort, HIGH);
        pinMode(_Hw[6].LedPort, OUTPUT);        
    }else{
        pinMode(_Hw[2].LedPort, INPUT);
        pinMode(_Hw[6].LedPort, INPUT);
    } 
    if(ver&0x10){
        digitalWrite(_Hw[3].LedPort, HIGH);
        pinMode(_Hw[3].LedPort, OUTPUT);
        digitalWrite(_Hw[7].LedPort, HIGH);
        pinMode(_Hw[7].LedPort, OUTPUT);
    }else{
        pinMode(_Hw[3].LedPort, INPUT);
        pinMode(_Hw[7].LedPort, INPUT);
    }     
    vTaskDelay(1000);
    if(ver&0x08){
        digitalWrite(_Hw[0].LedPort, LOW);
        pinMode(_Hw[0].LedPort, OUTPUT);
        digitalWrite(_Hw[4].LedPort, LOW);
        pinMode(_Hw[4].LedPort, OUTPUT);        
    }else{
        pinMode(_Hw[0].LedPort, INPUT);
        pinMode(_Hw[4].LedPort, INPUT);
    }
    if(ver&0x04){
        digitalWrite(_Hw[1].LedPort, LOW);
        pinMode(_Hw[1].LedPort, OUTPUT);
        digitalWrite(_Hw[5].LedPort, LOW);
        pinMode(_Hw[5].LedPort, OUTPUT);        
    }else{
        pinMode(_Hw[1].LedPort, INPUT);
        pinMode(_Hw[5].LedPort, INPUT);
    } 
    if(ver&0x02){
        digitalWrite(_Hw[2].LedPort, LOW);
        pinMode(_Hw[2].LedPort, OUTPUT);
        digitalWrite(_Hw[6].LedPort, LOW);
        pinMode(_Hw[6].LedPort, OUTPUT);        
    }else{
        pinMode(_Hw[2].LedPort, INPUT);
        pinMode(_Hw[6].LedPort, INPUT);
    } 
    if(ver&0x01){
        digitalWrite(_Hw[3].LedPort, LOW);
        pinMode(_Hw[3].LedPort, OUTPUT);
        digitalWrite(_Hw[7].LedPort, LOW);
        pinMode(_Hw[7].LedPort, OUTPUT);
    }else{
        pinMode(_Hw[3].LedPort, INPUT);
        pinMode(_Hw[7].LedPort, INPUT);
    }  
    vTaskDelay(1000);
	  taskENTER_CRITICAL();
/* 子设备状态刷新线程 */
		xReturn = xTaskCreate((TaskFunction_t )Sys_Task,   
							  (const char* )"SysTsk",                     /* Task name */
							  (uint16_t )256,   						              /* Stack size */
							  (void* )NULL,     						              /* parameter */
							  (osPriority_t) osPriorityNormal,  					/* priority */
							  (TaskHandle_t* )&SysTask_Handle); 
		if (pdPASS != xReturn){
				printf("SysTask create fail!\n");
		    return pdFAIL;
		} 
        
/* 8片LCM老化线程 */
    for(int ai = 0; ai< AGING_CH_MAX; ai++){
        xReturn = xTaskCreate((TaskFunction_t )Aging_Task,   
                    (const char* )"AgingTsk",                /* Task name */
                    (uint16_t )128,   						           /* Stack size */
                    (void* )AGB->Aging[ai],     						           /* parameter */
                    (osPriority_t) osPriorityNormal,  			 /* priority */
                    (TaskHandle_t* )&AgingTask_Handle[ai]); 
        if (pdPASS != xReturn){
            printf("Aging%d Task create fail!\n", ai);
            return pdFAIL;
        }         
    }
/* State LED 线程 */   
		xReturn = xTaskCreate((TaskFunction_t )Led_Task,   
							  (const char* )"LedTsk",                     /* Task name */
							  (uint16_t )256,   						              /* Stack size */
							  (void* )NULL,     						              /* parameter */
							  (osPriority_t) osPriorityNormal,  					/* priority */
							  (TaskHandle_t* )&LedTask_Handle); 
		if (pdPASS != xReturn){
				printf("LedTask create fail!\n");
		    return pdFAIL;
		}     
    taskEXIT_CRITICAL();
    return pdPASS;    
}  

/* 老化线程 */
void Aging_Task(void * argument){ 
AgingStru *aging = (AgingStru *)argument; 
  vTaskSetApplicationTaskTag( NULL, ( void * ) aging );
    while(1){
        KeyFunc(aging);
        AgingFunc(aging);
     }
}

/* 按键检测、状态指示线程 */
BOOL isKeyBusy(void){
BOOL bBusy = FALSE;
    for(int ai = 0; ai < AGING_CH_MAX; ai++){
          if(AGB->Aging[ai]->KeyCode != 0){
               bBusy = TRUE;
          }
    }
    return bBusy;     
}

void SerialRecvFunc(SerialStru *_this){
//  uint8 buf[2];
  if(_this->RecvBuf[0] == 0x18){
      rmtKeyCode = _this->RecvBuf[1];
      //buf[0] = 0xaa;
      //buf[1] = 0xcc;
      //PHX_SerialWriteP(Serial, buf, 2);
  }  
}

void Sys_Task(void * argument){
uint8 keyCode = 0; 
uint16 i=0;
   rmtKeyCode = 0;  
   while(1){     
      keyCode = GetKeyCode();
      if(keyCode !=0 || rmtKeyCode != 0){
          if(rmtKeyCode !=0)  { keyCode = rmtKeyCode; rmtKeyCode = 0; }
          if(isKeyBusy() == FALSE){
              for(int ai =0 ; ai<8; ai++){
                  AGB->Aging[ai]->KeyCode = keyCode; 
              }
              //PHX_SerialWriteP(Serial, &keyCode, 1);
          }   
      }
      if(Serial->bNewEvent == 1){
					for(i=0;i<Serial->RecvLen;i++)
					{
						DATA_tmp[i]=Serial->RecvBuf[i+1];
					}
          if(Serial->RecvBuf[0] <0x10){
               switch(Serial->RecvBuf[0]){
                 case 0x01:
                      NVIC_SystemReset();
                      break;
								 case 0x02:
											Flash_SectorErase(FLASH_SAVE_ADDR);
											Flash_PageProgram(FLASH_SAVE_ADDR,DATA_tmp);
											for(i=0;i<Serial->RecvLen;i++)
											{
												DATA_tmp[i]=0;
											}
											Flash_ReadByte(FLASH_SAVE_ADDR,DATA_tmp,Serial->RecvLen-1);
											CSZ_SerialWriteP(Serial,0x02,DATA_tmp, Serial->RecvLen); 
									 break;
               }
          }else{
               SerialRecvFunc(Serial);
          }  
          Serial->bNewEvent = 0;          
      }
            
   }
 }
 
 
