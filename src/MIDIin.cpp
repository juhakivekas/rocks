#include "MIDIin.h"
//pthread listener function
static void* listen(void* arg);

MIDIin::MIDIin(){
	listening = false;	
}

MIDIin::MIDIin(const char* device){
	//open device filedescriptor
	fd = open(device, O_RDONLY);
	if(fd == -1){
		perror("Error opening MIDI device");
		printf("Cannot open \"%s\"\n", device);
		exit(1);
	}

	//start listener thread
	int status = pthread_create(&listener, NULL, listen, (void*)this);
	if(status != 0){
		perror("Error starting listener thread: ");
		close(fd);
		exit(1);
	}
	listening = true;
	fprintf(stderr, "Opened MIDI device and started listener thread.\n");
}

MIDIin::~MIDIin(){
	if(listening){
		//kill listener thread
		pthread_cancel(listener);
		//close device filedescriptor
		close(fd);
		fprintf(stderr, "Closed MIDI device and killed listener thread.\n");
	}
}

int MIDIin::read_raw(){
	//read the raw data
	//XXX is this endianness dependent?
	int status = read(fd, (void*)raw, MIDI_RAW_LENGTH);
	if(status < 0){
		perror("Error reading MIDI device");
		exit(1);
	}
	//debug output
	//printf("Type: %02x Note: %02x Velocity: %02x\n", raw[0], raw[1], raw[2]);
	return 0;
}

#define CHAN_CTRL 0xb1
//#define NOTE_ON   0x91
//#define NOTE_OFF  0x81
#define NOTE_ON   0x90
#define NOTE_OFF  0x80
int MIDIin::format(){
	unsigned char type     = raw[0];
	unsigned char value    = raw[1];
	unsigned char velocity = raw[2];

	if(type == NOTE_ON){
		note = value;
	}else if(type == NOTE_OFF){
		note = 0;	
	}else if(type == CHAN_CTRL){
	}else{
		//lets just skip all the unecessary ones
		//fprintf(stderr, "Error in MIDI data formatting\n");
		//fprintf(stderr, "note:%d\n", value);
		//fprintf(stderr, "velo:%d\n", velocity);
		//something fucked up
		return 1;
	}
	return 0;
}

static void* listen(void* arg){
	MIDIin *g = (MIDIin*) arg;
	//read the controller indefinetly, break if the read fails.
	while(g->read_raw() == 0){
		g->format();
		//place format prints here if you want to check something
		//printf("%02x %02x\n", g->note, g->raw[1]);
	}
	return NULL;
}
