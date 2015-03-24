/* 
This is a realtime bytebeat JACK client object!
*/

#include "Jack_beater.h"

float uint8_to_float(int value){
	value &= 0xff;
	value -= 0x80;
	return ((float)value)/128.0;
}

int process(jack_nframes_t nframes, void *arg){
	Jack_beater *ctx = (Jack_beater*) arg;
	static int t_glob = 0;
	int i, j, t;
	sample_t *buffer;
	for(j=0; j<ctx->numbeats; j++){
		buffer = (sample_t*) jack_port_get_buffer (ctx->beats[j]->port, nframes);
		t = t_glob;
		for(i=0; i<(int) nframes; i++){
			buffer[i] = uint8_to_float(ctx->beats[j]->beat(t));
			t++;
		}
	}
	t_glob += nframes;
	return 0;
}

Jack_beater::Jack_beater(const char *client_name){
	numbeats = 0;
	jack_status_t status;
	client = jack_client_open(client_name, JackNoStartServer, &status);
	if (client == NULL) {
		fprintf (stderr, "JACK server not running?\n");
		exit(1);
	}
	jack_set_process_callback(client, process, (void*) this);
}

int Jack_beater::activate(){
	int status;
	status = jack_activate(client);
	if (status) {
		fprintf (stderr, "Error while activating JACK client, status:%d.\n", status);
		return 1;
	}
	return 0;
}

Jack_beater::~Jack_beater(){
	int status;
	status = jack_client_close(client);
	if(status){
		fprintf(stderr, "Error while closing JACK client, status:%d.\n", status);
	}
}

void Jack_beater::add_beat(Bytebeat *beat){
	beat->port = jack_port_register(client, beat->name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	beats[numbeats] = beat;
	numbeats++;	
}
