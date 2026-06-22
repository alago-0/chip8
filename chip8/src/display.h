// --------------------------------------------------------- //
// ---------------------- DISPLAY -------------------------- //
// --------------------------------------------------------- //


#ifndef DISPLAY_H
#define DISPLAY_H


#include <SDL2/SDL.h>
#include <stdbool.h>


#define LOGICAL_WIDTH 64
#define LOGICAL_HEIGHT 32
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

#define R_SET 102
#define G_SET 204
#define B_SET 255
#define A_SET 255

#define R_ERASE 0
#define G_ERASE 0
#define B_ERASE 0
#define A_ERASE 255


typedef struct
{
  SDL_Window    *window;
  SDL_Renderer  *renderer;
  SDL_Event     event;
  bool          is_running;
}
Display;


Display display_init(void);
void display_destroy(Display* display);
void display_clear_canvas(Display* display);
void display_draw_pixel(Display* display, 
                        unsigned short x, 
                        unsigned short y, 
                        unsigned short r,
                        unsigned short g,
                        unsigned short b,
                        unsigned short a);
void display_erase_pixel(Display* display,
                         unsigned short x,
                        unsigned short y);
void display_set_pixel(Display* display,
                       unsigned short x,
                       unsigned short y);
void display_render_present(Display* display);

#endif