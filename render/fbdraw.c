#include "fbdraw.h"

void fill_rect(fb_context* ctx, int x, int y, int w, int h, int RGBA){
	int i, j;
	int* pen =(int*) 
		(ctx->fbp 
		+ (x * ctx->vinfo.bits_per_pixel/8) 
		+ (y * ctx->vinfo.xres * ctx->vinfo.bits_per_pixel/8));

	for(j=0; j<h; j++){
		for(i=0; i<w; i++){
			*pen = RGBA;
			pen++;
		}
		pen += ctx->vinfo.xres - w;
	}
}

void plot_pixel(fb_context* ctx, int x, int y, int RGBA){
	*((int*)(ctx->fbp + (x + y * ctx->vinfo.xres) * ctx->vinfo.bits_per_pixel/8)) = RGBA;
}

void line(fb_context* ctx, int x1, int y1, int x2, int y2, int RGBA){
	int diff_x = x2 - x1;
	int diff_y = y2 - y1;
	int length = ((diff_x<0)?-diff_x:diff_x) + ((diff_y<0)?-diff_y:diff_y);
	float step_x = (float)diff_x / (float)length;
	float step_y = (float)diff_y / (float)length;
	
	int i;
	float x = (float) x1;
	float y = (float) y1;
	for(i=0; i<length; i++){
		plot_pixel(ctx, (int)x, (int)y, RGBA);
		x += step_x;
		y += step_y;
	}
}
