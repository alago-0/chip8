#include <stdio.h>
#include <chip8.h>


int main(int argc, char *argv[])
{
  // Supply arg for rom
  if (argc != 2)
  {
    printf("Supply rom path in first argument\n");
    return 1;
  }

  // Init
  Chip8 chip8 = chip8_init();
 
  // Load rom
  const char *path = argv[1];
  chip8_load_rom(&chip8,
                 path);

  // Run chip8
  chip8_run(&chip8);

  return 0;
}