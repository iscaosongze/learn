#ifndef  _FLASH_H_
#define  _FLASH_H_
#include "alpaca.h"

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define FLASH_Sector_0     ((uint16_t)0x0000) /*!< Sector Number 0 */
#define FLASH_Sector_1     ((uint16_t)0x0001) /*!< Sector Number 1 */
#define FLASH_Sector_2     ((uint16_t)0x002) /*!< Sector Number 2 */
#define FLASH_Sector_3     ((uint16_t)0x003) /*!< Sector Number 3 */
#define FLASH_Sector_4     ((uint16_t)0x004) /*!< Sector Number 4 */
#define FLASH_Sector_5     ((uint16_t)0x005) /*!< Sector Number 5 */
#define FLASH_Sector_6     ((uint16_t)0x006) /*!< Sector Number 6 */
#define FLASH_Sector_7     ((uint16_t)0x007) /*!< Sector Number 7 */
#define FLASH_Sector_8     ((uint16_t)0x008) /*!< Sector Number 8 */
#define FLASH_Sector_9     ((uint16_t)0x009) /*!< Sector Number 9 */
#define FLASH_Sector_10    ((uint16_t)0x00A) /*!< Sector Number 10 */
#define FLASH_Sector_11    ((uint16_t)0x00B) /*!< Sector Number 11 */


#define PAGE_SIZE       1020

#define STM32_FLASH_BASE    0x08000000 //STM32 FLASH 起始地址

#define FLASH_SAVE_ADDR     0x08060000	//写Flash的地址


void Flash_SectorErase(uint32_t addr);
void Flash_PageProgram(unsigned int addr, unsigned char *src);
void Flash_ReadWord(uint32_t addr, uint32_t *dst, uint32_t len);
void Flash_ReadByte(uint32_t addr, uint8_t *dst, uint32_t len);
void exitBoot(uint32 appStartAddr);

void Flash_test(void);

uint8 checkUserCode(uint32 usrAddr);

#endif

