#include "alpaca.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "def.h"
#include "stateled.h"
#include "stdlib.h"


#define  KEY_PWR_PIN     PB12   //KEY0
#define  KEY_UP_PIN      PB13   //KEY1
#define  KEY_DWN_PIN     PB14   //KEY2
#define  KEY_MOD_PIN     PB15   //KEY3
#define  KEY_LED_PIN     PB9


 

#define  KEY_PWR         (1<<0)
#define  KEY_UP          (1<<1)
#define  KEY_DWN         (1<<2)
#define  KEY_MOD         (1<<3)


uint8    f_keyDown;
 

AGBStru          _AGB, *AGB;
HwStru           _Hw[AGING_CH_MAX];
AgingStru        _Aging[AGING_CH_MAX];
I2CStru          _I2cDev[AGING_CH_MAX];
SerialStru       _Serial, *Serial;

extern void LcmListCreate(void);

void Board_Setup(void){
uint8 ai;
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);    
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN);
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN); 
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN);
  
  /* KeyBoard Initial*/
   pinMode(KEY_PWR_PIN, INPUT_PULLUP); 
   pinMode(KEY_UP_PIN,  INPUT_PULLUP);
   pinMode(KEY_DWN_PIN, INPUT_PULLUP);
   pinMode(KEY_MOD_PIN, INPUT_PULLUP); 
   pinMode(KEY_LED_PIN,  OUTPUT); 
    
   pinMode(GPI0_PIN, INPUT_PULLUP); 
    
  /* Aging Channel Initial */
    AGB = &_AGB;

    ai = 0;
    _Hw[ai].VddPort  = PD6;
    _Hw[ai].VledPort = PD7;
    _Hw[ai].WpPort   = PD0;
    _Hw[ai].BistPort = PD3;
    _Hw[ai].PwmPort  = PD14;
    _Hw[ai].BlenPort = PD5;
    _Hw[ai].VtpPort  = PA8;
    _Hw[ai].LedPort  = PB0;
    PHX_I2C_Initial(_I2cDev+0, PD1, PD2);
  
    ai = 1;
    _Hw[ai].VddPort  = PD14;
    _Hw[ai].VledPort = PD15;
    _Hw[ai].WpPort   = PD8;
    _Hw[ai].BistPort = PD11;
    _Hw[ai].PwmPort  = PD12;
    _Hw[ai].BlenPort = PD13;
     _Hw[ai].VtpPort  = PA9;
    _Hw[ai].LedPort  = PB1;
    PHX_I2C_Initial(_I2cDev+1, PD9, PD10);
     
    ai = 2;
    _Hw[ai].VddPort  = PE6;
    _Hw[ai].VledPort = PE7;
    _Hw[ai].WpPort   = PE0;
    _Hw[ai].BistPort = PE3;
    _Hw[ai].PwmPort  = PE4;
    _Hw[ai].BlenPort = PE5;
     _Hw[ai].VtpPort  = PA10;
    _Hw[ai].LedPort  = PC8;
    PHX_I2C_Initial(_I2cDev+ai, PE1, PE2);
    
    ai = 3;
    _Hw[ai].VddPort  = PE14;
    _Hw[ai].VledPort = PE15;
    _Hw[ai].WpPort   = PE8;
    _Hw[ai].BistPort = PE11;
    _Hw[ai].PwmPort  = PE12;
    _Hw[ai].BlenPort = PE13;
     _Hw[ai].VtpPort  = PA11;
    _Hw[ai].LedPort  = PC9;
    PHX_I2C_Initial(_I2cDev+3, PE9, PE10);
    
    ai = 4;
    _Hw[ai].VddPort  = PF6;
    _Hw[ai].VledPort = PF7;
    _Hw[ai].WpPort   = PF0;
    _Hw[ai].BistPort = PF3;
    _Hw[ai].PwmPort  = PF4;
    _Hw[ai].BlenPort = PF5;
     _Hw[ai].VtpPort  = PB4;
    _Hw[ai].LedPort  = PC10;
    PHX_I2C_Initial(_I2cDev+4, PF1, PF2);
    
    ai = 5;
    _Hw[ai].VddPort  = PF14;
    _Hw[ai].VledPort = PF15;
    _Hw[ai].WpPort   = PF8;
    _Hw[ai].BistPort = PF11;
    _Hw[ai].PwmPort  = PF12;
    _Hw[ai].BlenPort = PF13;
     _Hw[ai].VtpPort  = PB5;
    _Hw[ai].LedPort  = PC11;
    PHX_I2C_Initial(_I2cDev+5, PF9, PF10);
    
    ai = 6;
    _Hw[ai].VddPort  = PG6;
    _Hw[ai].VledPort = PG7;
    _Hw[ai].WpPort   = PG0;
    _Hw[ai].BistPort = PG3;
    _Hw[ai].PwmPort  = PG4;
    _Hw[ai].BlenPort = PG5;
     _Hw[ai].VtpPort  = PB6;
    _Hw[ai].LedPort  = PC12;
    PHX_I2C_Initial(_I2cDev+6, PG1, PG2);
    
    ai = 7;
    _Hw[ai].VddPort  = PG14;
    _Hw[ai].VledPort = PG15;
    _Hw[ai].WpPort   = PG8;
    _Hw[ai].BistPort = PG11;
    _Hw[ai].PwmPort  = PG12;
    _Hw[ai].BlenPort = PG13;
     _Hw[ai].VtpPort  = PB7;
    _Hw[ai].LedPort  = PC13;    
    PHX_I2C_Initial(_I2cDev+7, PG9, PG10);
    
    
    for(int ai =0; ai<AGING_CH_MAX; ai++){
        pinMode(_Hw[ai].VddPort,      OUTPUT);
        digitalWrite(_Hw[ai].VddPort, LOW);
        pinMode(_Hw[ai].VledPort,     OUTPUT);
        digitalWrite(_Hw[ai].VledPort,LOW);
        pinMode(_Hw[ai].WpPort,       OUTPUT);
        digitalWrite(_Hw[ai].WpPort,  LOW);
        pinMode(_Hw[ai].BistPort,     OUTPUT);
        digitalWrite(_Hw[ai].BistPort,LOW);
        pinMode(_Hw[ai].PwmPort,      OUTPUT);
        digitalWrite(_Hw[ai].PwmPort, LOW);
        pinMode(_Hw[ai].BlenPort,     OUTPUT);
        digitalWrite(_Hw[ai].BlenPort,LOW);
        pinMode(_Hw[ai].VtpPort,      OUTPUT);
        digitalWrite(_Hw[ai].VtpPort, LOW);        
        pinMode(_Hw[ai].LedPort,      INPUT);
        digitalWrite(_Hw[ai].LedPort, LOW);      
      
        _Aging[ai].Ch = ai;
        _Hw[ai].I2cDev = _I2cDev + ai;
        _Aging[ai].Hw = _Hw + ai;
        AGB->Aging[ai] = _Aging + ai;
    }

//LCM List
    LcmListCreate();
    for(int ai = 0; ai < AGING_CH_MAX; ai++){
       AGB->Aging[ai]->lcm = malloc(sizeof(LcmStru)); 
       AGB->Aging[ai]->bValid = FALSE; 
    }      
     Serial = &_Serial;
     Serial->Uart = 4;
     Serial->bRS485 = TRUE;
     Serial->RS485EnablePort = PA4;
     PHX_SerialInitial(Serial, NULL);
}

uint8 getKeyLevel(){
uint8 k = 0;
	if(digitalRead(KEY_PWR_PIN)==LOW)
		  k |= KEY_PWR;
	if(digitalRead(KEY_UP_PIN)==LOW)
		  k |= KEY_UP;	 
	if(digitalRead(KEY_DWN_PIN)==LOW)
		  k |= KEY_DWN;	 
	if(digitalRead(KEY_MOD_PIN)==LOW)
		  k |= KEY_MOD;	 
	return k;
}

unsigned char  GetKeyCode(void){
unsigned char keyNum;
uint8 buf[2];
  keyNum = getKeyLevel();
	if(keyNum == 0x00){
	    f_keyDown = 0;
      digitalWrite(KEY_LED_PIN, LOW);
	}else if(f_keyDown == 0){	
	      _delay_ms(40);
          keyNum = getKeyLevel();
		  if(keyNum !=0x00){
			 f_keyDown = 1;
             buf[0] = 0X18;
             buf[1] = keyNum;
             digitalWrite(KEY_LED_PIN, HIGH);
             PHX_SerialWriteP(Serial, buf, 2); 
		     return keyNum;
      }
  }
	return 0x0;
}

