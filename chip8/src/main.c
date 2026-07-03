#include <stdio.h>
#include <chip8.h>


int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Supply rom path in first argument\n");
    return 1;
  }

  Chip8 chip8 = chip8_init();
 
  // Load rom
  const char *path = argv[1];
  chip8_load_rom(&chip8,
                 path);

  return 0;
}