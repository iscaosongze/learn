#include <string.h>
//#include "function.h"
#include "PHX_Flash.h"

void (*jump2app)();
void JumpToBootloader(uint32 appStartAddr);
#define ENABLE_INT()	__set_PRIMASK(0)	/* Ê¹ÄÜÈ«¾ÖÖÐ¶Ï */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ½ûÖ¹È«¾ÖÖÐ¶Ï */


uint32_t GetSector(uint32_t Address){

  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

  return sector;
}



void Flash_WriteEnable(uint8_t enab){
	if(enab)   HAL_FLASH_Unlock();
	else       HAL_FLASH_Lock();		
}	

void Flash_SectorErase(uint32_t addr){
FLASH_EraseInitTypeDef pEraseInit;
uint32_t SectorError;

    pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    pEraseInit.Sector = GetSector(addr);
    pEraseInit.NbSectors = 1;
  
    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&pEraseInit,&SectorError);
    HAL_FLASH_Lock();
}	

void Flash_PageProgram(unsigned int addr, unsigned char *src){
	int i;
	Flash_WriteEnable(1);
	for(i=0; i<PAGE_SIZE; i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, src[i]);
	}	
	Flash_WriteEnable(0);
}	

void Flash_ReadWord(uint32_t addr, uint32_t *dst, uint32_t len)
{
uint32_t i;
	for(i=0; i<len; i++)
      dst[i] = *(__IO uint32_t*)(addr+4*i); 
}
void Flash_ReadByte(uint32_t addr, uint8_t *dst, uint32_t len){
uint16_t i;
	for(i=0; i<len; i++)
      dst[i] = *(__IO uint8_t*)(addr+i); 
}

void Flash_test(void){
#if 0	
  uint32_t buf[256];

	uint8_t *p = (uint8_t *)buf;
	for(int i=0; i<256; i++)  p[i] = 0;
    Flash_ReadWord(SPCU_APP_ADDR, buf, 64);	
	Flash_SectorErase(SPCU_APP_ADDR);	
	for(int i=0; i<256; i++)  p[i] = i;
	Flash_PageProgram(SPCU_APP_ADDR, (uint8*)(buf));
	for(int i=0; i<256; i++)  p[i] = 0;
	Flash_ReadWord(SPCU_APP_ADDR, buf, 64);	
#endif
  
}

uint8 checkUserCode(uint32 usrAddr){
  uint32 sp = *(uint32 *)usrAddr;
  if((sp & 0x2FFE0000) == 0x20000000)
       return TRUE;
  else
       return FALSE;		
}

void nvicDisableInterrupts() {
    for(int ai =0; ai<8; ai++){
        NVIC->ICER[ai] = 0xFFFFFFFF;
        NVIC->ICER[ai] = 0xFFFFFFFF;
    }
	SysTick->CTRL = 0x04;   /* disable the systick, which operates separately from nvic */

}

void exitBoot(uint32 appStartAddr){
 
	 typedef void (*funcPtr)(void);
	 if(checkUserCode(appStartAddr)==false)
		    NVIC_SystemReset();
     JumpToBootloader(appStartAddr);
     
#if 0
	 //jump2app=(void(*)())*(uint32_t*)(USER_APP_START_ADDR+4);
	 
	 
	 //uint32_t jumpAddr = SPCU_APP_ADDR + 0x04; /* reset ptr in vector table */
     uint32_t jumpAddr = *(volatile uint32_t *)(SPCU_APP_ADDR + 0x04); /* reset ptr in vector table */
	 funcPtr jump2app = (funcPtr) jumpAddr;
	 nvicDisableInterrupts();
	 //__set_MSP(*(volatile uint32_t*)SPCU_APP_ADDR);
     __set_MSP(*(__IO uint32_t*) (SPCU_APP_ADDR));
    //nvicDisableInterrupts();
 	jump2app();
#endif     
}

/*
*********************************************************************************************************
*	函 数 名: JumpToBootloader
*	功能说明: 跳转到系统BootLoader
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void JumpToBootloader(uint32 appStartAddr)
{
	uint32_t i=0;
	void (*SysMemBootJump)(void);        /* 声明一个函数指针 */
	__IO uint32_t BootAddr = appStartAddr; /* STM32F4的系统BootLoader地址 */

	/* 关闭全局中断 */
//	DISABLE_INT(); 
   
	/* 关闭滴答定时器，复位到默认值 */
//	SysTick->CTRL = 0;
//    SysTick->LOAD = 0;
//    SysTick->VAL = 0;

	/* 设置所有时钟到默认状态，使用HSI时钟 */
	//HAL_RCC_DeInit();

	/* 关闭所有中断，清除所有中断挂起标志 */
//	for (i = 0; i < 8; i++)
//	{
//		NVIC->ICER[i]=0xFFFFFFFF;
//		NVIC->ICPR[i]=0xFFFFFFFF;
//	}	

	/* 使能全局中断 */
	//ENABLE_INT();

	/* 跳转到系统BootLoader，首地址是MSP，地址+4是复位中断服务程序地址 */
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));

	/* 设置主堆栈指针 */
	__set_MSP(*(uint32_t *)BootAddr);
	
	/* 在RTOS工程，这条语句很重要，设置为特权级模式，使用MSP指针 */
	//__set_CONTROL(0);
//nvicDisableInterrupts();
	/* 跳转到系统BootLoader */
	SysMemBootJump(); 

	/* 跳转成功的话，不会执行到这里，用户可以在这里添加代码 */
	while (1)
	{

	}
}

 
