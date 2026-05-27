#include <display.h>


const unsigned short LOGICAL_WIDTH = 64;
const unsigned short LOGICAL_HEIGHT = 32;
const unsigned short WINDOW_WIDTH = 640;
const unsigned short WINDOW_HEIGHT = 320;


Display display_init()
{
  // SDL init
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("SDL_Init failed: %s\n", SDL_GetError());
    exit(1);
  }

  Display display;
  display.window = SDL_CreateWindow("Chip8",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH,
                                    WINDOW_HEIGHT,
                                    SDL_WINDOW_SHOWN);
  display.renderer = SDL_CreateRenderer(display.window,
                                        -1,
                                        SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(display.renderer,
                           LOGICAL_WIDTH,
                           LOGICAL_HEIGHT);
  
  display.is_running = true;
  return display;
}

void display_destroy(Display *display)
{
  SDL_DestroyRenderer(display->renderer);
  SDL_DestroyWindow(display->window);
  SDL_Quit();  
}