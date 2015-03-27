#ifndef jack_beater_h
#define jack_beater_h

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include <jack/jack.h>
#include <jack/transport.h>

#include "Bytebeat.h"

typedef jack_default_audio_sample_t sample_t;

class Beater{
public:
	jack_client_t *client;

	int numbeats;
	Bytebeat *beat[8];
	
	Beater(const char* name);
	~Beater();
	int activate();
	void add_beat(bytebeat_func func, const char* name);
};

#endif/*jack_beater_h*/
