#ifndef CORE_H
#define CORE_H

#include <stdint.h>


const uint16_t RAM_SIZE = 4096; // 0x000 to 0xFFF
const uint16_t RAM_MAX_VALUE = 4095; // 0xFFF
const uint16_t RAM_MIN_VALUE = 512; // 0x200 = 512 (most chip8 programs' start)


typedef struct
{
  uint8_t RAM[RAM_SIZE];
}
Core;


#endif