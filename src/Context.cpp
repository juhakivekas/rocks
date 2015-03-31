#include "Context.h"
#include <cstdio>

Context::Context(Gamepad *gamepad, BpmClock* clock){
	g = gamepad;
	c = clock;
}

Context::~Context(){
}

void Context::update(){
	data.lx = g->analog[LX]>>8;
	data.ly = g->analog[LY]>>8;
	data.rx = g->analog[RX]>>8;
	data.ry = g->analog[RY]>>8;
	data.d  = g->digital & 0xff;

	data.r = c->pulse;
}
