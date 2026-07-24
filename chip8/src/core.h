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

#define SCREEN_LOGICAL_WIDTH 64
#define SCREEN_LOGICAL_HEIGHT 32


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
  uint8_t    screen[SCREEN_LOGICAL_WIDTH][SCREEN_LOGICAL_HEIGHT];
}
Core;

Core core_init(void);
void core_set_to_default(Core* core);
void core_load_fonts(Core* core);
void core_load_rom(Core* core,
                   const char* path);

uint16_t core_fetch(Core* core);
void core_decode_execute(Core* core);

uint8_t core_get_first_nibble(uint16_t instruction);
uint8_t core_get_second_nibble(uint16_t instruction);
uint8_t core_get_third_nibble(uint16_t instruction);
uint8_t core_get_fourth_nibble(uint16_t instruction);
uint8_t core_get_second_byte(uint16_t instruction);
uint16_t core_get_second_third_fourth_nibbles(uint16_t instruction);

void core_clear_screen(Core* core); // 00E0

#endif