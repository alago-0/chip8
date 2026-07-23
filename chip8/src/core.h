// --------------------------------------------------------- //
// -------------------- CHIP 8 CORE ------------------------ //
// --------------------------------------------------------- //
#ifndef CORE_H
#define CORE_H


#include <stdint.h>


#define RAM_SIZE 4096
#define RAM_MAX 4095
#define RAM_START 512
#define FONT_START 0
#define STACK_SIZE 16
#define GENERAL_REGISTERS_NUMBER 16


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

Core core_init(void);
void core_set_to_default(Core* core);
void core_load_fonts(Core* core);
void core_load_rom(Core* core,
                   const char* path);

uint16_t core_fetch(Core* core);

#endif