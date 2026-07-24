#include <chip8.h>
#include <unistd.h>

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


void chip8_draw(Chip8* chip8)
{
  for (uint8_t x = 0; x < SCREEN_LOGICAL_WIDTH; ++x)
  {
    for (uint8_t y = 0; y < SCREEN_LOGICAL_HEIGHT; ++y)
    {
      if (chip8->core.screen[x][y] == true)
      {
        display_set_pixel(&chip8->display,
                          x,
                          y);
      }
      else
      {
        display_erase_pixel(&chip8->display,
                            x,
                            y);
      }
    }
  }

  display_render_present(&chip8->display);
}


void chip8_run(Chip8* chip8)
{
  while (chip8->display.is_running)
  {
    core_decode_execute(&chip8->core);
    chip8_draw(chip8);
    while (SDL_PollEvent(&chip8->display.event))
    {
      if (chip8->display.event.type == SDL_QUIT)
      {
        chip8->display.is_running = false;
      }
    }
  }

  display_destroy(&chip8->display);
}