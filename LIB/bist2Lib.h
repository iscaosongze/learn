#ifndef __BIST2LIB_H_
#define __BIST2LIB_H_
#include "types.h"

#define __VER_LOCATION   __attribute__((section(".ARM.__at_0x08021000")))


typedef enum{
    PWR_VDD,
    PWR_VLED,
    GPO_PWM,
    GPO_BLEN,
    GPO_BIST,
    GPO_WP,
    GPO_LED, 
    GPO_VTP,    
}GpoEnum;

typedef enum{
    GPI_0, 
}GpiEnum;

#define AGING_FLOW_MAX   16
#define LCM_NUM_MAX      16


typedef struct{
    uint8  maxSteps;     /**/
    uint32 loopTimes;     /**/
    void (*FlowFunc)(uint8 step);
}LoopStru;

typedef struct{
    uint8 TotalFlow;
    uint8 ChkNum;        /* 检查Pat数量 */
    uint8 *ChkList;      /* 检查Pattern */
    LoopStru Flow[AGING_FLOW_MAX];
    void (*EndFlowSeq)(void);
    void (*EnterInCheckMode)(void);    
}FlowStru;

typedef struct{
    char     Name[16];
    uint32   IdAddr;        /* ROM 地址 */
    const FlowStru *Flow; 
}LcmStru;

typedef struct{
    void (*PowerOffSeq)(void);  
    void (*PowerOnSeq)(void);
    void (*Pattern)(uint8 p); 
    LcmStru* (*ValidCheck)(void);    
}TconStru;

#define LcmListCount(l)     (sizeof(l)/sizeof(LcmStru))
    
void Lcm_Add(LcmStru *lcm);
void Tcon_Add(TconStru *tcon);
void _delay_ms(uint32 t);
void DelayL(uint32 t);
void SetGPIO(uint8 gpo, uint8 val);
uint8 GetGPI(uint8 gpi);
BOOL I2C_Write(uint8 slvAddr, uint8 *txbuf, uint16 txLen);
BOOL I2C_Read(uint8 slvAddr, uint8 *txbuf, uint16 txLen, uint8 *rxbuf, uint16 rxLen);
 
BOOL StrCompare(char *p1, char *p2, uint16 len);
#endif
