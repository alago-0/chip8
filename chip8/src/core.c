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


void core_clear_screen(Core *core) // 00E0
{
  memset(core->screen, 0, sizeof(core->screen));
}


bool core_screen_get_pixel(Core* core,
                           uint8_t x,
                           uint8_t y)
{
  if (x >= SCREEN_LOGICAL_WIDTH)
  {
    printf("core_screen_get_pixel failed - x overflow (%u)\n", x);
    exit(1);
  }

  if (y >= SCREEN_LOGICAL_HEIGHT)
  {
    printf("core_screen_get_pixel faild - y overflow (%u)\n", y);
    exit(1);
  }

  return core->screen[x][y];
}


void core_screen_set_pixel(Core* core,
                           uint8_t x,
                           uint8_t y,
                           bool value)
{
  if (x >= SCREEN_LOGICAL_WIDTH)
  {
    printf("core_screen_set_pixel failed - x overflow (%u)\n", x);
    exit(1);
  }

  if (y >= SCREEN_LOGICAL_HEIGHT)
  {
    printf("core_screen_set_pixel faild - y overflow (%u)\n", y);
    exit(1);
  }

  core->screen[x][y] = value;
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
  uint8_t first_nibble = core_instruction_get_first_nibble(instruction);
  uint8_t second_nibble = core_instruction_get_second_nibble(instruction);
  uint8_t third_nibble = core_instruction_get_third_nibble(instruction);
  uint8_t fourth_nibble = core_instruction_get_fourth_nibble(instruction);
  uint16_t second_third_fourth_nibbles = core_instruction_get_second_third_fourth_nibbles(instruction);
  uint8_t second_byte = core_instruction_get_second_byte(instruction);

  switch (first_nibble)
  {
    case 0x0:
    {
      switch (second_third_fourth_nibbles)
      {
        case 0x0E0: // 00E0
        {
          core_clear_screen(core);
          break;
        }
        case 0x0EE: // 00EE
        {
          core_set_pc(core,
                      core_pop_from_stack(core));
          break;
        }
        default:
        {
          break;
        }
      }
      break;
    }
   
    case 0x1: // 1NNN
    {
      core_set_pc(core,
                  second_third_fourth_nibbles);
      break;
    }
    
    case 0x2: // 2NNN
    {
      core_push_to_stack(core, 
                         core->pc);
      core_set_pc(core, 
                  second_third_fourth_nibbles);
      break;
    }

    case 0x3: // 3XNN
    {
      core_skip_instruction_if_equal(core,
                                     core_get_register_v(core, second_nibble),
                                     second_byte);
      break;
    }

    case 0x4: // 4XNN
    {
      core_skip_instruction_if_not_equal(core,
                                         core_get_register_v(core, second_nibble),
                                         second_byte);
      break;
    }

    case 0x5: // 5XY0
    {
      core_skip_instruction_if_equal(core,
                                     core_get_register_v(core, second_nibble),
                                     core_get_register_v(core, third_nibble));
      break;
    }

    case 0x6: // 6XNN
    {
      core_set_register_v(core,
                          second_nibble,
                          second_byte);
      break;
    }

    case 0x7: // 7XNN
    {
      core_add_to_register_v(core,
                             second_nibble,
                             second_byte);
      break;
    }

    case 0x8: // 8...
        switch (fourth_nibble)
        {
          case 0x0: // 8XY0
          {
            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                    third_nibble));
            break;
          }
          case 0x1: // 8XY1
          {
            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                    second_nibble) |
                                core_get_register_v(core,
                                                    third_nibble));
            break;
          }
          case 0x2: // 8XY2
          {
            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                    second_nibble) &
                                core_get_register_v(core,
                                                    third_nibble));
            break;
          }
          case 0x3: // 8XY3
          {
             core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                    second_nibble) ^
                                core_get_register_v(core,
                                                    third_nibble));
            break;
          }

          case 0x4: // 8XY4
          {
            size_t sum = (size_t) core_get_register_v(core,
                                                      second_nibble) + core_get_register_v(core,
                                                                                           third_nibble);
            sum != (uint8_t)sum ? 
                   core_set_register_v(core, 0xF, 1) :
                   core_set_register_v(core, 0xF, 0);
            core_set_register_v(core,
                                second_nibble,
                                (uint8_t) sum);
            break;
          }
          
          case 0x5: // 8XY5
          {
            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                    second_nibble) -
                                core_get_register_v(core,
                                                    third_nibble));
            break;
          }
         
          case 0x6: // 8XY6
          {
            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                     third_nibble));
            bool is_bit_one = core_get_register_v(core,
                                                  second_nibble) & 1;
            is_bit_one == true ? 
                          core_set_register_v(core,
                                              0xF,
                                              1) :
                          core_set_register_v(core,
                                              0xF,
                                              0);
            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                    second_nibble) >> 1);
            break;
          }
          case 0x7: // 8XY7
          {
            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                    third_nibble) -
                                core_get_register_v(core,
                                                    second_nibble));           
            break;
          }
          
          case 0xE: // 8XYE
          {

            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                     third_nibble));
            bool is_bit_one = (core_get_register_v(core,
                                                   second_nibble) >> 7) & 1;
            is_bit_one == true ? 
                          core_set_register_v(core,
                                              0xF,
                                              1) :
                          core_set_register_v(core,
                                              0xF,
                                              0);
            core_set_register_v(core,
                                second_nibble,
                                core_get_register_v(core,
                                                    second_nibble) << 1);
            break;
          }
        }
        break;
    
    case 0x9: // 9XY0
    {
      core_skip_instruction_if_not_equal(core,
                                         core_get_register_v(core, second_nibble),
                                         core_get_register_v(core, third_nibble));
      break;
    }

    case 0xA: // ANNN
    {
      core_set_index(core,
                     second_third_fourth_nibbles);
      break;
    }

    case 0xD: // DXYN
    {
      core_draw(core,
                second_nibble,
                third_nibble,
                fourth_nibble);
      break;
    }

    case 0xF: // F...
    {
      switch (second_byte)
      {
        case 0x33: // FX33
        {
          uint8_t number = core_get_register_v(core,
                                               second_nibble);
          core_set_ram(core,
                       core_get_index(core),
                       number / 100);
          core_set_ram(core,
                       core_get_index(core) + 1,
                       (number / 10) % 10);
          core_set_ram(core,
                       core_get_index(core) + 2,
                       number % 10);
          break;
        }
        case 0x55: // FX55
          for (uint8_t i = 0; i <= second_nibble; ++i)
          {
            uint16_t address = core_get_index(core) + i;
            uint8_t value = core_get_register_v(core, i);
            core_set_ram(core,
                         address,
                         value);
          } 
          break;
        case 0x65: // FX65
          for (uint8_t i = 0; i <= second_nibble; ++i)
          {
            uint16_t address = core_get_index(core) + i;
            uint8_t value = core_get_ram(core, address);
            core_set_register_v(core,
                                i,
                                value);
          }
          break;
        case 0x1E: // FX1E
        {
          size_t sum = (size_t) core_get_index(core) + core_get_register_v(core, second_nibble);
          sum != (uint16_t)sum ? 
                 core_set_register_v(core, 0xF, 1) :
                 core_set_register_v(core, 0xF, 0);
          core_set_index(core,
                         (uint16_t)sum);
          break;
        }
      }
      break;
    }
      
    default:
    {
      break;
    }
  }
}


uint8_t core_instruction_get_first_nibble(uint16_t instruction)
{
  return (instruction & 0xF000) >> 12;
}


uint8_t core_instruction_get_second_nibble(uint16_t instruction)
{
  return (instruction & 0x0F00) >> 8;
}


uint8_t core_instruction_get_third_nibble(uint16_t instruction)
{
  return (instruction & 0x00F0) >> 4;
}


uint8_t core_instruction_get_fourth_nibble(uint16_t instruction)
{
  return instruction & 0xF;
}


uint8_t core_instruction_get_second_byte(uint16_t instruction)
{
  return instruction & 0xFF;
}


uint16_t core_instruction_get_second_third_fourth_nibbles(uint16_t instruction)
{
  return instruction & 0xFFF;
}


uint8_t core_get_ram(Core* core,
                     uint16_t address)
{
  if (address >= RAM_MAX)
  {
    printf("core_get_ram failed - ram overflow");
    exit(1);
  }

  return core->ram[address];
}


void core_set_ram(Core* core,
                  uint16_t address,
                  uint8_t value)
{
  if (address >= RAM_MAX)
  {
    printf("core_set_ram failed - ram overflow");
    exit(1);
  }

  core->ram[address] = value;
}


void core_set_pc(Core* core,
                 uint16_t address) // 1NNN
{
  if (core->pc >= RAM_MAX)
  {
    printf("core_set_pc failed - ram overflow");
    exit(1);
  }

  core->pc = address;
}


void core_set_register_v(Core* core,
                         uint8_t register_number,
                         uint8_t value) // 6XNN
{
  if (register_number >= GENERAL_REGISTERS_NUMBER)
  {
    printf("core_set_register_v failed - register_number overflow\n");
    exit(1);
  }

  core->v[register_number] = value;
}


uint8_t core_get_register_v(Core* core,
                            uint8_t register_number)
{
  if (register_number >= GENERAL_REGISTERS_NUMBER)
  {
    printf("core_get_register_v failed - register_number overflow\n");
    exit(1);
  }

  return core->v[register_number];
}


void core_skip_instruction_if_equal(Core* core,
                                    uint8_t value1,
                                    uint8_t value2) // 3XNN 5XY0
{
  if (value1 == value2)
  {
    core->pc += 2;
  }
}


void core_skip_instruction_if_not_equal(Core* core,
                                        uint8_t value1,
                                        uint8_t value2) // 4XNN, 9XY0
{
  if (value1 != value2)
  {
    core->pc += 2;
  }
}


void core_add_to_register_v(Core* core,
                            uint8_t register_number,
                            uint8_t value) // 7XNN
{
  if (register_number >= GENERAL_REGISTERS_NUMBER)
  {
    printf("core_set_register_v failed - register_number overflow\n");
    exit(1);
  }

  core->v[register_number] += value;
}


void core_push_to_stack(Core* core,
                        uint16_t value)
{
  if (core->sp >= STACK_SIZE)
  {
    printf("core_push_to_stack failed - sp overflow");
    exit(1);
  }

  core->stack[core->sp++] = value;
}


uint16_t core_pop_from_stack(Core* core)
{
  if (core->sp < 0)
  {
    printf("core_pop_from_stack failed - sp overflow");
    exit(1);
  }

  return core->stack[--core->sp];
}


uint16_t core_get_index(Core* core)
{
  return core->i;
}


void core_set_index(Core* core,
                    uint16_t value) // ANNN
{
  core->i = value;
}


void core_draw(Core* core,
               uint8_t second_nibble,
               uint8_t third_nibble,
               uint8_t fourth_nibble) // DXYN
{
  uint8_t x = core_get_register_v(core, second_nibble) % SCREEN_LOGICAL_WIDTH;
  uint8_t y = core_get_register_v(core, third_nibble) % SCREEN_LOGICAL_HEIGHT;
  core_set_register_v(core,
                      0xF,
                      0);

  uint8_t row = 0;
  while (row < fourth_nibble)
  {
    uint8_t draw_y = y + row;
    if (draw_y == SCREEN_LOGICAL_HEIGHT)
    {
      break;
    }

    uint8_t sprite_byte = core_get_ram(core,
                                       core->i + row);
    for (int8_t pixel = 7; pixel >=0; --pixel)
    {
      uint8_t draw_x = x + 7 - pixel;
      if (draw_x == SCREEN_LOGICAL_WIDTH)
      {
        break;
      }

      uint8_t bit = (sprite_byte >> pixel) & (uint8_t)1;

      // Draw
      if (bit)
      {
        if (core_screen_get_pixel(core,
                                  draw_x,
                                  draw_y))
        {
          core_screen_set_pixel(core,
                                draw_x,
                                draw_y,
                                false);
          core_set_register_v(core,
                              0xF,
                              1);
        }
        else
        {
          core_screen_set_pixel(core,
                                draw_x,
                                draw_y,
                                true);
        }
      }
    }

    ++row;
  }
}