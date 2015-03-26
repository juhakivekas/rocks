#include "Framebuffer.h"

//--------CONSTRUCTOR AND DESTRUCTOR
Framebuffer::Framebuffer(const char* device){
	int status;

	// Open the file for reading and writing
	fd = open(device, O_RDWR);
	if (fd < 0) {
		perror("Error opening framebuffer");
		fprintf(stderr, "Cannot open framebuffer device \"%s\"\n", device);
		exit(1);
	}
	fprintf(stderr, "Framebuffer device opened.\n");

	// Get variable screen information
	status = ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
	if(status != 0) {
		perror("Error reading variable framebuffer information: ");
		exit(1);
	}

	printf("Framebuffer: %dx%d, %dbpp\n",
		vinfo.xres,
		vinfo.yres,
		vinfo.bits_per_pixel
	);
 
	// Get fixed screen information
	status = ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
	if (status){
		perror("Error reading fixed framebuffer information");
		exit(1);
	}

	// map fb to user mem 
	fbp = (char*)mmap(
		NULL, 
		finfo.smem_len, 
		PROT_READ | PROT_WRITE, 
		MAP_SHARED, 
		fd, 
		0
	);

	if(fbp == MAP_FAILED){
		perror("Memory mapping the framebuffer failed");
		close(fd);
		exit(1);
	}
}

Framebuffer::~Framebuffer(){
	munmap(fbp, finfo.smem_len);
	close(fd);
	printf("Closed framebuffer device.\n");
}

//-------DRAWING FUNCTIONS

void Framebuffer::clear(){
	memset(fbp, 0x00000000, vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8);
}

void Framebuffer::pixel(int x, int y, int ARGB){
	//This is a slowpoke, don't use this in long loops
	*((int*)(fbp + (x + y * vinfo.xres) * vinfo.bits_per_pixel/8)) = ARGB;
}

void Framebuffer::line(int x1, int y1, int x2, int y2, int ARGB){
	int diff_x = x2 - x1;
	int diff_y = y2 - y1;
	int length = ((diff_x<0)?-diff_x:diff_x) + ((diff_y<0)?-diff_y:diff_y);
	float step_x = (float)diff_x / (float)length;
	float step_y = (float)diff_y / (float)length;
	
	int i;
	float x = (float) x1;
	float y = (float) y1;
	for(i=0; i<length; i++){
		pixel((int)x, (int)y, ARGB);
		x += step_x;
		y += step_y;
	}
}

void Framebuffer::rect(int x, int y, int w, int h, int ARGB){
	if(x < 0){ w += x; x = 0;}
	if(y < 0){ h += y; y = 0;}
	if(x > (int) vinfo.xres){ return; }
	if(y > (int) vinfo.yres){ return; }
	if(x+w > (int) vinfo.xres){ w = vinfo.xres-x; }
	if(y+h > (int) vinfo.yres){ h = vinfo.yres-y; }

	int i, j;
	int* pen =(int*) 
		(fbp 
		+ (x * vinfo.bits_per_pixel/8) 
		+ (y * vinfo.xres * vinfo.bits_per_pixel/8));

	for(j=0; j<h; j++){
		for(i=0; i<w; i++){
			*pen = ARGB;
			pen++;
		}
		pen += vinfo.xres - w;
	}
}
