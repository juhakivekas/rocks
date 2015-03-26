#include "Framebuffer.h"
#include "Gamepad.h"

#define framebuffer_path "/dev/fb0"
#define gamepad_path "/dev/input/js0"

int main(){
	Framebuffer f = Framebuffer(framebuffer_path);
	Gamepad g = Gamepad(gamepad_path);
	int color;
	float x,y,xx,yy;
	x = ((float) g.analog[RX] / (float)(1<<16))*f.vinfo.xres;
	y = ((float) g.analog[RY] / (float)(1<<16))*f.vinfo.yres;
	int i=0;
	f.clear();
	while(i<1000){
		xx=x;
		yy=y;
		x = ((float) g.analog[RX] / (float)(1<<16))*f.vinfo.xres;
		y = ((float) g.analog[RY] / (float)(1<<16))*f.vinfo.yres;
		//x = g.analog[RX]>>7;
		//y = g.analog[RY]>>7;
		color = ((g.analog[LX]>>8) + (g.analog[LY]&0xff00));
		color <<= (g.digital>>5) & 0x0f;
		//f.line((int) xx, (int) yy, (int) x, (int) y, color);
		f.rect(((int)x & ~0xf)-8, ((int)y & ~0xf)-8, 16, 16, color);
		//5ms
		usleep(5000);
	}
	return 0;
}
