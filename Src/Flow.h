#ifndef __DEF_H_
#define __DEF_H_

#include "bist2Lib.h"

typedef enum{ST_PWROFF, ST_CHECK, ST_AGING, ST_AGING_FINISH} AgStateEnum;

typedef struct{
    I2CStru         *I2cDev;    
    uint8           VddPort;
    uint8           VledPort;
    uint8           WpPort;
    uint8           BistPort; 
    uint8           PwmPort;
    uint8           BlenPort;
    uint8           LedPort; 
}HwStru;

typedef struct{
    uint8           Ch;
    HwStru          *Hw; 
    LcmStru         *lcm;
    uint8           LedState;
    AgStateEnum     StSys;
    uint8           pCode;
    uint8           KeyCode;
    uint8           iStep;
    uint8           iFlow;
    uint32          m_loopDelay;
    uint32          StepCount;
}AgingStru;

typedef struct{
    uint8       Addr;
    AgingStru   *Aging[8];  
    uint8       LcmNum;
    LcmStru     *LcmList[LCM_NUM_MAX];
}AGBStru;

extern AGBStru  *AGB;

#endif
