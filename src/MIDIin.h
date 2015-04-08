#ifndef midiin_h
#define midiin_h

#include <pthread.h>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define	MIDI_RAW_LENGTH 3

class MIDIin{
public:
	//FORMATTED DATA
	//just one note for the time being
	unsigned char note;

	//construct and destruct
	MIDIin();
	MIDIin(const char* device);
	~MIDIin();

//-----------------------------------------------------------------------------
	//XXX everything from here on should be private
	//XXX but it has to be available for pthread to work
	//RAW DATA
	uint8_t raw[MIDI_RAW_LENGTH];	//raw USB-HID data


	//these have to be public so that they can be called from within a thread
	int read_raw();
	int format();

private:	
	bool listening;
	//device and thread overhead
	int fd;						//device file descriptor
	pthread_t listener;			//listener thread ID
};


#endif/*midiin_h*/
