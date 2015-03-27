/* 
This is a realtime bytebeat JACK client object!
*/

#include "Beater.h"

float uint8_to_float(int value){
	value &= 0xff;
	value -= 0x80;
	return ((float)value)/128.0;
}

int process(jack_nframes_t nframes, void *arg){
	//the bytebeat counter, initialized once
	static int t_glob = 0;

	Beater *beater = (Beater*) arg;
	//update the interface context variables
	beater->context->update();

	int i, j, t;
	sample_t *buffer;
	for(j=0; j<beater->numbeats; j++){
		buffer = (sample_t*) jack_port_get_buffer (beater->beat[j]->port, nframes);
		t = t_glob;
		for(i=0; i<(int) nframes; i++){
			buffer[i] = uint8_to_float(beater->beat[j]->func(t, &beater->context->data));
			t++;
		}
	}
	t_glob += nframes;
	return 0;
}

Beater::Beater(Context * ctx, const char *client_name){
	context = ctx;
	numbeats = 0;
	jack_status_t status;
	client = jack_client_open(client_name, JackNoStartServer, &status);
	if (client == NULL) {
		fprintf(stderr, "JACK server not running?\n");
		exit(1);
	}
	jack_set_process_callback(client, process, (void*) this);

	//construct a context
	fprintf(stderr, "JACK client opened.\n");
}

int Beater::activate(){
	int status;
	status = jack_activate(client);
	if (status) {
		fprintf(stderr, "Error while activating JACK client, status:%d.\n", status);
		return 1;
	}
	fprintf(stderr, "JACK client activated.\n");
	return 0;
}

Beater::~Beater(){
	int status;
	status = jack_client_close(client);
	if(status){
		fprintf(stderr, "Error while closing JACK client, status:%d.\n", status);
	}
}

void Beater::add_beat(bytebeat_func func, const char* name){
	Bytebeat *b = new Bytebeat(func, name);
	b->port = jack_port_register(client, b->name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	beat[numbeats] = b;
	numbeats++;
}
