#include <display.h>


const unsigned short LOGICAL_WIDTH = 64;
const unsigned short LOGICAL_HEIGHT = 32;
const unsigned short WINDOW_WIDTH = 640;
const unsigned short WINDOW_HEIGHT = 320;

const unsigned short R_SET = 102;
const unsigned short G_SET = 204;
const unsigned short B_SET = 255;
const unsigned short A_SET = 255;

const unsigned short R_ERASE = 0;
const unsigned short G_ERASE = 0;
const unsigned short B_ERASE = 0;
const unsigned short A_ERASE = 255;


Display display_init(void)
{
  // SDL init
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("SDL_Init failed: %s\n", SDL_GetError());
    exit(1);
  }

  Display display;
 
  // Window
  display.window = SDL_CreateWindow("Chip8",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH,
                                    WINDOW_HEIGHT,
                                    SDL_WINDOW_SHOWN);
  if (!display.window)
  {
    printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
    exit(1);
  }

  // Renderer
  display.renderer = SDL_CreateRenderer(display.window,
                                        -1,
                                        SDL_RENDERER_ACCELERATED);
  if (!display.renderer)
  {
    printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_RenderSetLogicalSize(display.renderer,
                           LOGICAL_WIDTH,
                           LOGICAL_HEIGHT);
  clear_canvas(&display);
  SDL_RenderPresent(display.renderer);
  
  display.is_running = true;
  return display;
}

void display_destroy(Display *display)
{
  SDL_DestroyRenderer(display->renderer);
  SDL_DestroyWindow(display->window);
  SDL_Quit();  
}

void clear_canvas(Display *display)
{
  SDL_SetRenderDrawColor(display->renderer,
                         R_ERASE,
                         G_ERASE,
                         B_ERASE,
                         A_ERASE);
  SDL_RenderClear(display->renderer);
}

void draw_pixel(Display *display,
                unsigned short x,
                unsigned short y,
                unsigned short r,
                unsigned short g,
                unsigned short b,
                unsigned short a)
{
  if (x < LOGICAL_WIDTH &&
      y < LOGICAL_HEIGHT)
  {
    SDL_SetRenderDrawColor(display->renderer,
                           r,
                           g,
                           b,
                           a);
    SDL_RenderDrawPoint(display->renderer,
                        x,
                        y);
  }
}

void erase_pixel(Display *display,
                 unsigned short x,
                 unsigned short y)
{
  draw_pixel(display,
             x,
             y,
             R_ERASE,
             G_ERASE,
             B_ERASE,
             A_ERASE);
}

void set_pixel(Display *display,
               unsigned short x,
               unsigned short y)
{
  draw_pixel(display,
             x,
             y,
             R_SET,
             G_SET,
             B_SET,
             A_SET);
}