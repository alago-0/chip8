#include <core.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


Core core_init(void)
{
  // New instance
  Core core;

  // Set all to 0
  core_set_to_default(&core);

  // Load fonts
  core_load_fonts(&core);

  return core;
}


void core_set_to_default(Core *core)
{
  // Set all to 0
  memset(core->ram, 0, sizeof(core->ram));
  memset(core->stack, 0, sizeof(core->stack));
  core->i = 0;
  core->delay_timer = 0;
  core->sound_timer = 0;
  core->pc = RAM_START; 
  core->sp = 0;
  memset(core->v, 0, sizeof(core->v));
  core_clear_screen(core);
}


void core_load_fonts(Core *core)
{
  const uint8_t font_bytes[80] = 
  {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  for (unsigned short i = 0; i < 80; ++i)
  {
    core->ram[FONT_START + i] = font_bytes[i]; 
  }
}


void core_load_rom(Core* core,
                   const char* path)
{
  // Open rom
  FILE *f = fopen(path, "rb");
  if (!f)
  {
    printf("core_load_rom failed - f == null\n");
    exit(1);
  }

  // Load rom
  size_t max = RAM_SIZE - RAM_START;
  fread(&core->ram[RAM_START], 1, max, f);

  bool is_too_big = fgetc(f) != EOF;
  fclose(f);
  if (is_too_big)
  {
    printf("core_load_rom failed - rom too big\n");
    exit(1); 
  }
}


uint16_t core_fetch(Core* core)
{
  // Check for overflow
  if (core->pc + 2 > RAM_SIZE)
  {
    printf("core_fetch failed - ram overflow\n");
    exit(1);
  }

  uint8_t first_byte = core->ram[core->pc++];
  uint8_t second_byte = core->ram[core->pc++];

  uint16_t instruction = first_byte;
  instruction = (instruction << 8) | second_byte;
  return instruction;
}


void core_decode_execute(Core* core)
{
  // Fetch
  uint16_t instruction = core_fetch(core);

  // Decode and execute
  switch (instruction)
  {
    case 0x00E0:
      core_clear_screen(core);
      break;
    
    default:
      break;
  }
}


uint8_t core_get_first_nibble(uint16_t instruction)
{
  return (instruction >> 12) & 0xF;
}


uint8_t core_get_second_nibble(uint16_t instruction)
{
  return ((instruction << 4) >> 12) & 0xF;
}


uint8_t core_get_third_nibble(uint16_t instruction)
{
  return ((instruction << 8) >> 12) & 0xF;
}


uint8_t core_get_fourth_nibble(uint16_t instruction)
{
  return (instruction << 12) >> 12;
}


uint8_t core_get_second_byte(uint16_t instruction)
{
  return (instruction << 8) >> 8;
}


uint16_t core_get_second_third_fourth_nibbles(uint16_t instruction)
{
  return (instruction << 4) >> 4;
}


void core_clear_screen(Core *core) // 00E0
{
  memset(core->screen, 0, sizeof(core->screen));
}