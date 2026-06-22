#include <stdio.h>
#include <display.h>
#include <core.h>

int main(void)
{
  printf("hello world\n");

  Core chip8_core = core_init();
  printf("%u", chip8_core.ram[0]);

  Display display = display_init();

  display_set_pixel(&display, 10, 10);
  display_render_present(&display);
  while (display.is_running)
  {
    while (SDL_PollEvent(&display.event))
    {
      if (display.event.type == SDL_QUIT)
      {
        display.is_running = false;
      }
    }
  }
  display_destroy(&display);
  return 0;
}