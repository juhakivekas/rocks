#ifndef bytebeat_h
#define bytebeat_h

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <jack/jack.h>
#include <jack/transport.h>

typedef char (*bytebeat_func)(int t);

class Bytebeat{
public:
	jack_port_t* port;		//JACK output port
	char name[64];			//the name that shows up on the JACK port
	bytebeat_func beat;		//bytebeat function pointer, t is time, m is MIDI

	Bytebeat(const char* name, bytebeat_func beat);
	~Bytebeat();
};


#endif/*bytebeat_h*/
