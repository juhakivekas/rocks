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

class Jack_beater{
public:
	jack_client_t *client;
	int numbeats;
	Bytebeat *beats[8];
	
	Jack_beater(const char* name);
	~Jack_beater();
	int activate();
	void add_beat(Bytebeat* beat);
};

#endif/*jack_beater_h*/
