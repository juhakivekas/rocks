#ifndef context_h
#define context_h

#include "Gamepad.h"
#include "MIDIin.h"
#include "BpmClock.h"

typedef struct char_data {
	unsigned int r;
	unsigned char lx, ly;
	unsigned char rx, ry;
	unsigned char d;
	//define the data you want to use in your bytebeats here
}char_data;

class Context{
public:
	//define the needed sources of input for the bytebeats
	Gamepad *g;
	MIDIin *m;
	BpmClock *c;

	//this is what we update, get, set and pass to the beats
	char_data data;
	//the bytebeat counter
	//int t;
	float t;
	float t_diff;
	


	//construct and destruct
	Context(Gamepad* gamepad, BpmClock *clock, MIDIin *midiin);
	~Context();
	void update();
};

#endif/*context_h*/
