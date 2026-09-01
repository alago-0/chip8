// --------------------------------------------------------- //
// -------------------- CHIP 8 CORE ------------------------ //
// --------------------------------------------------------- //
#ifndef CORE_H
#define CORE_H


#include <stdint.h>
#include <stdbool.h>


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
  bool       screen[SCREEN_LOGICAL_WIDTH][SCREEN_LOGICAL_HEIGHT];
}
Core;

Core core_init(void);
void core_set_to_default(Core* core);
void core_load_fonts(Core* core);
void core_load_rom(Core* core,
                   const char* path);

bool core_screen_get_pixel(Core* core,
                           uint8_t x,
                           uint8_t y);
void core_screen_set_pixel(Core* core,
                           uint8_t x,
                           uint8_t y,
                           bool value);
void core_clear_screen(Core* core); // 00E0


uint16_t core_fetch(Core* core);
void core_decode_execute(Core* core);

uint8_t core_instruction_get_first_nibble(uint16_t instruction);
uint8_t core_instruction_get_second_nibble(uint16_t instruction);
uint8_t core_instruction_get_third_nibble(uint16_t instruction);
uint8_t core_instruction_get_fourth_nibble(uint16_t instruction);
uint8_t core_instruction_get_second_byte(uint16_t instruction);
uint16_t core_instruction_get_second_third_fourth_nibbles(uint16_t instruction);

uint8_t core_get_ram(Core* core,
                     uint16_t address);
void core_set_ram(Core* core,
                  uint16_t address,
                  uint8_t value);

void core_set_pc(Core* core,
                 uint16_t address); // 1NNN
void core_skip_instruction_if_equal(Core* core,
                                    uint8_t value1,
                                    uint8_t value2); // 3XNN 5XY0
void core_skip_instruction_if_not_equal(Core* core,
                                        uint8_t value1,
                                        uint8_t value2); // 4XNN 9XY0

void core_set_register_v(Core* core,
                         uint8_t register_number,
                         uint8_t value); // 6XNN
uint8_t core_get_register_v(Core* core,
                            uint8_t register_number);
void core_add_to_register_v(Core* core,
                            uint8_t register_number,
                            uint8_t value); // 7XNN

void core_push_to_stack(Core* core,
                        uint16_t value);
uint16_t core_pop_from_stack(Core* core);

uint16_t core_get_index(Core* core);
void core_set_index(Core* core,
                    uint16_t value); // ANNN
                  
void core_draw(Core* core,
               uint8_t second_nibble,
               uint8_t third_nibble,
               uint8_t fourth_nibble); // DXYN

#endif