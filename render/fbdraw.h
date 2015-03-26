#ifndef fbdraw_functions
#define fbdraw_functions

#include "fbinit.h" //for fb_context

void fill_rect(fb_context* ctx, int x, int y, int w, int h, int ARGB);
void plot_pixel(fb_context* ctx, int x, int y, int RGBA);
void line(fb_context* ctx, int x1, int y1, int x2, int y2, int ARGB);

#endif /*fbdraw_functions*/
