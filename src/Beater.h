#ifndef beater_h
#define beater_h

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include <jack/jack.h>
#include <jack/transport.h>

#include "Bytebeat.h"
#include "Context.h"

typedef jack_default_audio_sample_t sample_t;

class Beater{
public:
	
	jack_client_t *client;
	char client_name[64];

	Context *context;

	int numbeats;
	Bytebeat *beat[8];
	
	Beater(Context* ctx, const char* name);
	~Beater();
	void add_beat(bytebeat_func func, const char* name);
};

#endif/*beater_h*/
