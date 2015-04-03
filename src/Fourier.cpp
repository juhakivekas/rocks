#include "Fourier.h"

int process(jack_nframes_t nframes, void* arg){
	Fourier *f = (Fourier*) arg;
	sample_t *buffer = (sample_t*) jack_port_get_buffer(f->port, nframes);
	
	return 0;
}

Fourier::Fourier(const char* name){
	strncpy(client_name, name, 64);
	jack_status_t status;
	int status_int;

	client = jack_client_open(client_name, JackNoStartServer, &status);
	if (client == NULL) {
		fprintf(stderr, "JACK server not running?\n");
		exit(1);
	}
	fprintf(stderr, "JACK client opened.\n");

	jack_set_process_callback(client, process, (void*) this);
	status_int = jack_activate(client);
	if (status_int) {
		fprintf(stderr, "Error while activating JACK client, status:%d.\n", status);
		exit(1);
	}
	fprintf(stderr, "JACK client activated.\n");
	jack_port_register(client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	sprintf(port_name, "%s:input", client_name);
	jack_connect(client, "system:capture_1", port_name);
	jack_connect(client, "system:capture_2", port_name);
	fprintf(stderr, "JACK port registerd and connected.\n");
}

Fourier::~Fourier(){
	int status;
	status = jack_client_close(client);
	if(status){
		fprintf(stderr, "Error while closing JACK client, status:%d.\n", status);
	}	
}
