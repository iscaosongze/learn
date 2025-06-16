#ifndef __DEF_H_
#define __DEF_H_

#include "bist2Lib.h"

#define TCON_NUM_MAX     16

#define AGING_CH_MAX      8
#define LCM_NUM_MAX      16
#define AGING_IN_STEP     255

#define  KEY_PWR         (1<<0)
#define  KEY_UP          (1<<1)
#define  KEY_DWN         (1<<2)
#define  KEY_MOD         (1<<3)

#define  GPI0_PIN     PA7

typedef enum{ST_PWROFF, ST_CHECK, ST_AGING, ST_AGING_FINISH} AgStateEnum;

typedef struct{
    I2CStru         *I2cDev;     /**/
    uint8           VddPort;     /* VSHD */
    uint8           VledPort;    /* VBL_EN */
    uint8           WpPort;      /* WP   */
    uint8           BistPort;    /* BIST */
    uint8           PwmPort;     /* PWM  */
    uint8           BlenPort;    /* BL_EN*/
    uint8           VtpPort;     /* VLED_EN */
    uint8           LedPort; 
}HwStru;

typedef struct{
    uint8           Ch;
    BOOL            bValid;         /*  */
    HwStru          *Hw; 
    TconStru        *Tcon;
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
    uint8       TconNum;
    TconStru    *TconList[TCON_NUM_MAX];
}AGBStru;

extern AGBStru  *AGB;
extern SerialStru *Serial;
unsigned char GetKeyCode(void);

#endif
