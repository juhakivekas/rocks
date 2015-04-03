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
	Beater *beater = (Beater*) arg;
	//update the interface context variables
	beater->context->update();

	int i, j;
	float t;
	sample_t *buffer;
	for(j=0; j<beater->numbeats; j++){
		buffer = (sample_t*) jack_port_get_buffer (beater->beat[j]->port, nframes);
		t = beater->context->t;
		for(i=0; i<(int) nframes; i++){
			buffer[i] = uint8_to_float(beater->beat[j]->func((int)t, &beater->context->data));
			//t++;
			t += beater->context->t_diff;
		}
	}

	//here is documented a fixed bug relating to floating point errors:
	//printf("%f\t%f\n", t - beater->context->t, nframes*beater->context->t_diff);
	beater->context->t = t; 
	return 0;
}

Beater::Beater(Context * ctx, const char *name){
	context = ctx;
	strncpy(client_name, name, 64);
	numbeats = 0;
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
	beat[numbeats] = b;
	numbeats++;
	//register the new port
	b->port = jack_port_register(client, b->name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	//connect the new port
	char portname[128];
	sprintf(portname, "%s:%s", client_name, b->name);
	jack_connect(client, portname, "system:playback_1");
	jack_connect(client, portname, "system:playback_2");
}
