// --------------------------------------------------------- //
// -------------------- CHIP 8 CORE ------------------------ //
// --------------------------------------------------------- //
#ifndef CORE_H
#define CORE_H

#include <stdint.h>

const uint16_t RAM_SIZE = 4096; // 0x000 to 0xFFF
const uint16_t RAM_MAX = 4095; // 0xFFF
const uint16_t RAM_START = 512; // 0x200 = 512 (most chip8 programs' start)
const uint8_t STACK_SIZE = 16;
const uint8_t GENERAL_REGISTERS_NUMBER = 16;

typedef struct
{
  uint8_t    ram[RAM_SIZE];
  uint8_t    v[GENERAL_REGISTERS_NUMBER];
  uint16_t   i;
  uint8_t    delay_timer;
  uint8_t    sound_timer;
  uint16_t   pc;
  uint8_t    sp;
  uint16_t   stack[STACK_SIZE]; 
}
Core;

#endif