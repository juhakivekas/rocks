#ifndef fbinit
#define fbinit

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

typedef struct fb_context{
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	char *fbp;		//framebuffer pointer
	int fbfd;		//framebuffer file descriptor
}fb_context;

fb_context* init_framebuffer(char* device);
void close_framebuffer(fb_context* ctx);

#endif /*fbinit*/
