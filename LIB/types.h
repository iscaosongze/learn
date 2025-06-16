#ifndef _ALPACA_TYPES_H_
#define _ALPACA_TYPES_H_

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uint8_t;
typedef unsigned int uint32_t; 

typedef unsigned char BOOL;

#define HIGH        0x1
#define LOW         0x0
#ifndef TRUE
  #define TRUE        0x01
#endif
#ifndef FALSE
 #define FALSE       0x00
#endif

#define True          1
#define False         0
enum{ 
	INPUT = 0,   /*Same as INPUT_FLOATING*/
	INPUT_ANALOG,
	INPUT_PULLUP,
	INPUT_PULLDOWN,
	INPUT_FLOATING,
	
  OUTPUT, 
	OUTPUT_OPEN_DRAIN,
	
	PWM,
	PWM_OPEN_DRAIN,
};

typedef enum {
    IN_LOW = 0, 
    IN_RISING, 
    IN_FALLING, 
    IN_HIGH 
} InLevelEnum;

#ifndef true
  #define true  0x1
  #define false 0x0
#endif
	
#ifndef NULL
  #define NULL 0
#endif

#define lowByte(w)                     ((w) & 0xFF)
#define highByte(w)                    (((w) >> 8) & 0xFF)
#define bitRead(value, bit)            (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)             ((value) |= (1UL << (bit)))
#define bitClear(value, bit)           ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : \
                                                   bitClear(value, bit))
#define bit(b)                         (1UL << (b))
	
#ifndef __weak
   #define __weak   __attribute__((weak))
#endif /* __weak */
	
#endif
