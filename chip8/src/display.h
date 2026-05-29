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

extern const unsigned short R_SET;
extern const unsigned short G_SET;
extern const unsigned short B_SET;
extern const unsigned short A_SET;

extern const unsigned short R_ERASE;
extern const unsigned short G_ERASE;
extern const unsigned short B_ERASE;
extern const unsigned short A_ERASE;


typedef struct
{
  SDL_Window    *window;
  SDL_Renderer  *renderer;
  SDL_Event     event;
  bool          is_running;
}
Display;


Display display_init(void);
void display_destroy(Display *display);
void display_clear_canvas(Display *display);
void display_draw_pixel(Display *display, 
                        unsigned short x, 
                        unsigned short y, 
                        unsigned short r,
                        unsigned short g,
                        unsigned short b,
                        unsigned short a);
void display_erase_pixel(Display *display,
                         unsigned short x,
                        unsigned short y);
void display_set_pixel(Display *display,
                       unsigned short x,
                       unsigned short y);
void display_render_present(Display *display);

#endif