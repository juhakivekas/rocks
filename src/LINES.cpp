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

int NUMLINES = 3;
int NUMBARS = 4;

typedef struct point{
	float x, y;
}point;
typedef point vector;

typedef struct line{
	point a, b;
}line;

typedef struct range{
	float s, e;//start and end
}range;


line lines[32] = {
	{{0.0, 0.1},{0.1, 0.9}},
	{{0.2, 0.05},{0.2, 0.6}},
	{{1.0, 0.3},{0.8, 1.0}}
};

range bars[128] = {
	{0.0, 0.1},
	{0.2, 0.4},
	{0.45, 0.5},
	{0.8, 0.95}
};
//initialize the rendering data
void init_data(){
	srand(123);
	int i,j,k;
	for(i=0; i<5; i++){
		lines[i].a.x = rand();
	}
}

#define BORDER 0.0
#include "Gamepad.h"
Gamepad g = Gamepad("/dev/input/js0");
void Draw(float ratio) {
	glColor3f(1.0, 1.0, 1.0);
	int i,j;
	vector ab;
	for(i=0; i<NUMBARS; i++){
		glBegin(GL_QUAD_STRIP);
		for(j=0; j<NUMLINES; j++){
			ab.x = lines[j].b.x - lines[j].a.x;
			ab.y = lines[j].b.y - lines[j].a.y;
			glVertex3f(
				lines[j].a.x + bars[i].s*ab.x,
				lines[j].a.y + bars[i].s*ab.y,
				0);
			glVertex3f(
				lines[j].a.x + bars[i].e*ab.x,
				lines[j].a.y + bars[i].e*ab.y,
				0);
		}
		glEnd();
	}
	//glLineWidth(1);
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
	glOrtho(0, aspect_ratio, 0, 1.0, 1.0, 100.0);

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
	init_data();
	while(true) {
		ExposeFunc(); 
		usleep(1000);
		if(g.button(10)){
			ExitProgram();
		}
	}
}
