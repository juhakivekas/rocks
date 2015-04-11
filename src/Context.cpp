#include "Context.h"
#include "freq.h"
#include <cstdio>

Context::Context(Gamepad *gamepad, BpmClock *clock, MIDIin *midiin){
	t = 0;
	t_diff = 1;
	g = gamepad;
	c = clock;
	m = midiin;
}

Context::~Context(){
}

/*we need to simulate overflowing the counter in
order to avoid accumulated floating point error,
that could set the instrument out of tune*/
#define T_MAX ((float)(1<<16))

#define JACK_SAMPLERATE 48000

void Context::update(){

	data.lx = g->analog[LX]>>8;
	data.ly = g->analog[LY]>>8;
	data.rx = g->analog[RX]>>8;
	data.ry = g->analog[RY]>>8;
	data.d  = g->digital & 0xff;

	//data.r = c->pulse;
	data.note = m->note;
	t_diff = (freq[data.note]*128.0)/JACK_SAMPLERATE;

	//simulated overflow to keep floating point accuracy high
	if(t>T_MAX) t -= T_MAX;
}
