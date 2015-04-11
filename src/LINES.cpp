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
#include<X11/Xatom.h>
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

int numlines = 10;
int numbars;

typedef struct point{
	float x, y;
}point;
typedef point vector;

typedef struct line{
	//the points
	point a, b;
	//the movement vectors
	vector av, bv;
	vector avv, bvv;
}line;

typedef struct range{
	float s, e;//start and end
	float r,g,b;
}range;


line lines[32] = {
	{{0.0, 0.1},{0.1, 0.9}},
	{{1.0, 0.3},{0.8, 1.0}}
};

range bars[128] = {
	{0.0, 0.1},
	{0.2, 0.4},
	{0.45, 0.5},
	{0.8, 0.95}
};

#include "Gamepad.h"
Gamepad g = Gamepad("/dev/input/js0");

//Gamepad g = Gamepad();

float randf(float min, float max){
	return min + ((rand()%1000)/1000.0) * (max-min);
}

#define upper_screen 1.5
#define lower_screen -0.5
#define upper_velocity 0.1
#define lower_velocity -0.1
void limit_to(float *f, float upper, float lower){
	if(*f > upper) *f = upper;
	if(*f < lower) *f = lower;
}

void refresh_data(){
	int i;
	float noise = g.analogf(RX)*2;
	if(g.button(5)) noise = 1;
	if(g.button(7)) noise = 2;
	if(g.button_toggle(2)) noise += 0.02;
	if(g.button_toggle(3)) noise += 0.04;
	if(g.button_toggle(4)) noise += 0.08;
	noise *= 0.01;
	for(i=0; i<numlines; i++){
		lines[i].avv.x += randf(-noise, noise);
		lines[i].avv.y += randf(-noise, noise);
		lines[i].bvv.x += randf(-noise, noise);
		lines[i].bvv.y += randf(-noise, noise);
		
		lines[i].avv.x *= 0.5;
		lines[i].avv.y *= 0.5;
		lines[i].bvv.x *= 0.5;
		lines[i].bvv.y *= 0.5;

		lines[i].av.x += lines[i].avv.x;
		lines[i].av.y += lines[i].avv.y;
		lines[i].bv.x += lines[i].bvv.x;
		lines[i].bv.y += lines[i].bvv.y;

		lines[i].av.x *= 0.9;
		lines[i].av.y *= 0.9;
		lines[i].bv.x *= 0.9;
		lines[i].bv.y *= 0.9;

		lines[i].a.x += lines[i].av.x;
		lines[i].a.y += lines[i].av.y;
		lines[i].b.x += lines[i].bv.x;
		lines[i].b.y += lines[i].bv.y;
	}

	//make sure shit is not going too far
	for(i=0; i<numlines; i++){
		limit_to(&lines[i].a.x, upper_screen, lower_screen);
		limit_to(&lines[i].a.y, upper_screen, lower_screen);
		limit_to(&lines[i].b.x, upper_screen, lower_screen);
		limit_to(&lines[i].b.y, upper_screen, lower_screen);
		limit_to(&lines[i].av.x, upper_velocity, lower_velocity);
		limit_to(&lines[i].av.y, upper_velocity, lower_velocity);
		limit_to(&lines[i].bv.x, upper_velocity, lower_velocity);
		limit_to(&lines[i].bv.y, upper_velocity, lower_velocity);
	}

	i=0;	
	float f=0.0;
	static int prev_six = 0;
	static int curr_six = 0;
	curr_six = g.button(6);
	if(curr_six && !prev_six){
		while(f<1.0 && i < 128){
			//start and end
			f += randf(0,g.analogfp(LX)/10.0);
			bars[i].s = f;
			f += randf(0,g.analogfp(LY)/10.0)+0.001;
			bars[i].e = f;
		
			bars[i].r = randf(0.0, 1.0);	
			bars[i].g = randf(0.0, 1.0);	
			bars[i].b = randf(0.0, 1.0);	
			i++;
		}
		numbars = i;
	}
	prev_six = curr_six;
	usleep(10000);
}
#define BORDER 0.0
void Draw(float ratio) {
	int i,j;
	vector ab;
	if(g.button_toggle(1)){
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		glVertex3f(-2,-2,-1);
		glVertex3f(2,-2,-1);
		glVertex3f(2,2,-1);
		glVertex3f(-2,2,-1);
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
	}else{
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-2,-2,-1);
		glVertex3f(2,-2,-1);
		glVertex3f(2,2,-1);
		glVertex3f(-2,2,-1);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
	}
	for(i=0; i<numbars; i++){
		glBegin(GL_QUAD_STRIP);
		if(g.button_toggle(8))glColor3f(bars[i].r, bars[i].g, bars[i].b);
		for(j=0; j<numlines; j++){
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
	//  DRAW STUFF                 //
	/////////////////////////////////
	refresh_data();
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
	srand(1234);
	CreateWindow();
	SetupGL();
	while(true) {
		ExposeFunc(); 
		usleep(1000);
		if(g.button(10)){
			ExitProgram();
		}
	}
}
