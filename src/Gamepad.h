#ifndef gamepad_h
#define gamepad_h

#include <pthread.h>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define	RAW_LENGTH 8

//left joystick
#define LX 0x00
#define LY 0x01
//right joystick
#define RX 0x02
#define RY 0x03
//directional pad
#define PADX 0x04
#define PADY 0x05

//message types
#define BUTTON 0x01
#define	ANALOG 0x02
#define BUTTON_INIT 0x81
#define ANALOG_INIT 0x82


class Gamepad{
public:
	//RAW DATA
	uint8_t raw[RAW_LENGTH];	//raw USB-HID data

	//FORMATTED DATA
	//timestamp
	uint32_t time;

	//digital/button inputs are numbered as in the controller
	//ie. the button labeled "1" is bit number 1
	uint32_t digital;

	//analog channels ordered according to the axis enumerator
	int32_t analog[6];

	//construct and destruct
	Gamepad(const char* device);
	~Gamepad();

	//getter for nice looking button reads
	int button(int i){
		return (digital & (1<<i)) != 0;
	};

	//these have to be public so that they can be called from within a thread
	int read_raw();
	int format();

private:	
	//device and thread overhead
	int fd;						//device file descriptor
	pthread_t listener;			//listener thread ID
};

void* listen(void* arg);

#endif/*gamepad_h*/
