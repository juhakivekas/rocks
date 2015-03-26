#include "fbinit.h"

fb_context* init_framebuffer(char* device)
{
	//make a context for this framebuffer
	fb_context* ctx = malloc(sizeof(fb_context));

	// Open the file for reading and writing
	ctx->fbfd = open(device, O_RDWR);
	if (!ctx->fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		return NULL;
	}
	printf("The framebuffer device was opened successfully.\n");


	// Get variable screen information
	if (ioctl(ctx->fbfd, FBIOGET_VSCREENINFO, &ctx->vinfo)) {
		printf("Error reading variable information.\n");
		return NULL;
	}
	printf("Framebuffer: %dx%d, %dbpp\n",
		ctx->vinfo.xres,
		ctx->vinfo.yres,
		ctx->vinfo.bits_per_pixel
	);
 
	// Get fixed screen information
	if (ioctl(ctx->fbfd, FBIOGET_FSCREENINFO, &(ctx->finfo))) {
		printf("Error reading fixed information.\n");
	}


	// map fb to user mem 
	ctx->fbp = (char*)mmap(0, 
		ctx->finfo.smem_len, 
		PROT_READ | PROT_WRITE, 
		MAP_SHARED, 
		ctx->fbfd, 
		0
	);

	return ctx;
}

void close_framebuffer(fb_context* ctx){
	munmap(ctx->fbp, ctx->finfo.smem_len);
	close(ctx->fbfd);
	printf("Closed framebuffer device.\n");
}
