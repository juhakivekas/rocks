#ifndef context_h
#define context_h

#include "Gamepad.h"

typedef struct char_data {
	char lx, ly;
	char rx, ry;
	char d;
	//define the data you want to use in your bytebeats here
}char_data;

class Context{
public:
	//define the needed sources of input for the bytebeats
	Gamepad *g;

	//this is what we update, get, set and pass to the beats
	char_data data;

	//construct and destruct
	Context(Gamepad* gpad);
	~Context();
	void update();
};

#endif/*context_h*/
