#ifndef Framebuffer_h
#define Framebuffer_h

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

class Framebuffer{
public:
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	char *fbp;		//framebuffer pointer

	Framebuffer(const char* device);
	~Framebuffer();

	void clear();
	void pixel(int x, int y, int RGBA);
	void line(int x1, int y1, int x2, int y2, int ARGB);
	void rect(int x, int y, int w, int h, int ARGB);
private:
	int fd;			//framebuffer file descriptor
};

#endif/*Framebuffer_h*/
