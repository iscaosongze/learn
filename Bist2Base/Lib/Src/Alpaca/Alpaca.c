#include "alpaca.h"

uint32_t Byte_to_Int(uint8_t *byt){
 uint32_t dat;
 uint8_t *pc = (uint8_t*)(&dat);   
     pc[0] = byt[0];
     pc[1] = byt[1];
     pc[2] = byt[2];
     pc[3] = byt[3];
     return dat;
}
 
