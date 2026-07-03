#include <chip8.h>


Chip8 chip8_init()
{
  Core core = core_init();
  Display display = display_init();

  Chip8 chip8;
  chip8.core = core;
  chip8.display = display;

  return chip8;
}


void chip8_destroy(Chip8* chip8)
{
  display_destroy(&chip8->display);
}


void chip8_load_rom(Chip8* chip8,
                    const char* path)
{
  core_load_rom(&chip8->core,
                path);
}

