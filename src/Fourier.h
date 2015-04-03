#ifndef fourier_h
#define fourier_h

#include <jack/jack.h>
#include <jack/transport.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define SPECTRUM_DIVISIONS 16
#define FREQ_MIN 0
#define FREQ_STEP 100

#define BUFFER_LEN 1024

typedef jack_default_audio_sample_t sample_t;
class Fourier{
public:
	int spectrum[SPECTRUM_DIVISIONS];
	Fourier(const char* name);
	~Fourier();

	jack_client_t *client;
	char client_name[64];
	jack_port_t *port;
	char port_name[64];
};

int process(jack_nframes_t nframes, void* arg);
#endif/*fourier_h*/
