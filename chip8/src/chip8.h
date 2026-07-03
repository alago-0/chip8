// --------------------------------------------------------- //
// ----------------------- CHIP 8 -------------------------- //
// --------------------------------------------------------- //
#ifndef CHIP8_H
#define CHIP8_H


#include <core.h>
#include <display.h>


typedef struct
{
  Core core;
  Display display;
}
Chip8;


Chip8 chip8_init();
void chip8_destroy(Chip8* chip8);
void chip8_load_rom(Chip8* chip8,
                    const char* path);


#endif