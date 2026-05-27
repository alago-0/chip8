// --------------------------------------------------------- //
// ---------------------- DISPLAY -------------------------- //
// --------------------------------------------------------- //


#ifndef DISPLAY_H
#define DISPLAY_H


#include <SDL2/SDL.h>
#include <stdbool.h>


extern const unsigned short LOGICAL_WIDTH;
extern const unsigned short LOGICAL_HEIGHT;
extern const unsigned short WINDOW_WIDTH;
extern const unsigned short WINDOW_HEIGHT;


typedef struct
{
  SDL_Window    *window;
  SDL_Renderer  *renderer;
  SDL_Event     event;
  bool          is_running;
}
Display;


Display display_init();
void display_destroy(Display *display);


#endif