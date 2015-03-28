#include "Context.h"
#include <cstdio>

Context::Context(Gamepad *gpad){
	g = gpad;
}

Context::~Context(){
}

void Context::update(){
	data.lx = g->analog[LX]>>8;
	data.ly = g->analog[LY]>>8;
	data.rx = g->analog[RX]>>8;
	data.ry = g->analog[RY]>>8;
	data.d  = g->digital & 0xff;
	//fprintf(stderr, "context.lx: %02x\n", data.lx);	
}
