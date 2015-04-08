// -- Written in C++ -- //

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>
#include<X11/Xlib.h>
#include<X11/XKBlib.h>
#include<GL/glx.h>
#include<GL/glext.h>
#include<GL/glu.h>
///////////////////////////////////////////////////////////////////////////////
//  GLOBAL IDENTIFIERS                                                       //
///////////////////////////////////////////////////////////////////////////////
Display					*dpy;
Window					root, win;
GLint					att[]	= { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo				*vi;
GLXContext				glc;
Colormap				cmap;
XSetWindowAttributes	swa;
XWindowAttributes		wa;
XEvent					xev;

GLfloat					rotation_matrix[16];
///////////////////////////////////////////////////////////////////////////////
//  DRAW GRAPHICS                                                            //
///////////////////////////////////////////////////////////////////////////////

#define square(x,y,z,size) \
	glVertex3f((x)-(size), (y-size), (z));\
	glVertex3f((x)+(size), (y-size), (z));\
	glVertex3f((x)+(size), (y+size), (z));\
	glVertex3f((x)-(size), (y+size), (z));\

#define BORDER 0.0
#include "Gamepad.h"
Gamepad g = Gamepad("/dev/input/js0");
void Draw(float ratio) {
	glBegin(GL_QUADS);

	float xstep = ratio/13.0;
	float ystep = 1.0/13.0;
	float x=xstep/2, y=ystep/2;
	float size;
	int i, j;
	glColor3f(1.0, 1.0, 1.0);
	for(j=0; j<13; j++){
		for(i=0; i<13; i++){
			if((i^j)&1){
				//set 5
				glColor3f(0.7, 1.0, 0.0);
				size = 0;
				if(g.button(5)) size += (i&j);
				if(g.button(6)) size += ((12-i)&(12-j));
				size *= g.analog[LX]/40000000.0;
				
				square(x,y,1,size);
			}else if(i&j&1){
				//set 4
				glColor3f(0.5, 1.0, 1.0);
				square(x,y,3,(((i+j)&3)+1)*g.analog[LY]/8000000.0);
			}else if(((i^j)&0x2)){
				//set 2
				glColor3f(1.0, 0.5, 0.0);
				size=8;
				if(g.button(8)) size += (((i+1)*j)&0xf);
				square(x,y,4,size*g.analog[RY]/60000000.0);
			}else if(((i|j)&0x3)==0){
				//set 1
				glColor3f(0.3, 0.0, 0.3);
				if(g.button(7)){
					square(x,y,5,g.analog[RX]/2000000.0);
				}
			}
			x += xstep;
		}
		y += ystep;
		x = xstep/2;
	}
	glEnd();
}
///////////////////////////////////////////////////////////////////////////////
//  EXPOSURE FUNCTION                                                        //
///////////////////////////////////////////////////////////////////////////////
void ExposeFunc() {
	float aspect_ratio;
	/////////////////////////////////
	//  RESIZE VIEWPORT            //
	/////////////////////////////////
	XGetWindowAttributes(dpy, win, &wa);
	glViewport(0, 0, wa.width, wa.height);
	aspect_ratio = (float)(wa.width) / (float)(wa.height);
	/////////////////////////////////////////
	//  SETUP PROJECTION & MODELVIEW       //
	/////////////////////////////////////////
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-BORDER, aspect_ratio+BORDER, -BORDER, 1.0+BORDER, 1., 100.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//looking from above, y facing up.
	gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
	/////////////////////////////////
	//  DRAW CUBE                  //
	/////////////////////////////////
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw(aspect_ratio);
	/////////////////////////////////
	//  DISPLAY TIME, FPS etc.     //
	/////////////////////////////////
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (float)wa.width, 0, (float)wa.height, -1., 1.);

	/////////////////////////////////
	//  SWAP BUFFERS               //
	/////////////////////////////////
	glXSwapBuffers(dpy, win);
}
///////////////////////////////////////////////////////////////////////////////
//  CREATE A GL CAPABLE WINDOW                                               //
///////////////////////////////////////////////////////////////////////////////
void CreateWindow() {

	if((dpy = XOpenDisplay(NULL)) == NULL) {
		printf("\n\tcannot connect to x server\n\n");
		exit(0);
	}

	root = DefaultRootWindow(dpy);
	
	if((vi = glXChooseVisual(dpy, 0, att)) == NULL) {
		printf("\n\tno matching visual\n\n");
		exit(0);
	}
			 
	if((cmap = XCreateColormap(dpy, root, vi->visual, AllocNone)) == 0) {
		printf("\n\tcannot create colormap\n\n");
		exit(0);
	}
			 
	swa.event_mask = KeyPressMask;
	swa.colormap	= cmap;
	win = XCreateWindow(dpy, root, 0, 0, 700, 700, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	XStoreName(dpy, win, "KOOLLA");
	XMapWindow(dpy, win);
}
///////////////////////////////////////////////////////////////////////////////
//  SETUP GL CONTEXT                                                         //
///////////////////////////////////////////////////////////////////////////////
void SetupGL() {
	/////////////////////////////////////////////////
	//  CREATE GL CONTEXT AND MAKE IT CURRENT      //
	/////////////////////////////////////////////////
	if((glc = glXCreateContext(dpy, vi, NULL, GL_TRUE)) == NULL) {
		printf("\n\tcannot create gl context\n\n");
		exit(0);
	 }

	glXMakeCurrent(dpy, win, glc);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05, 0.00, 0.1, 1.00);
}
///////////////////////////////////////////////////////////////////////////////
//  EXIT PROGRAM                                                             //
///////////////////////////////////////////////////////////////////////////////
void ExitProgram() {
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	exit(0);
}
///////////////////////////////////////////////////////////////////////////////
//  MAIN PROGRAM                                                             //
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){
	CreateWindow();
	SetupGL();
	while(true) {
		ExposeFunc(); 
		usleep(1000);
	}
}
