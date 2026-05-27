#include <stdio.h>
#include <display.h>

int main(void)
{
  printf("hello world\n");

  Display display = display_init();
  while (display.is_running)
  {
    while (SDL_PollEvent(&display.event))
    {
      if (display.event.type == SDL_QUIT)
      {
        display.is_running = false;
        display_destroy(&display);
        return 0;
      }
    }
  }

  return 0;
}