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
	//FORMATTED DATA
	//timestamp
	uint32_t time;

	//digital/button inputs are numbered as in the controller
	//ie. the button labeled "1" is bit number 1
	uint32_t digital;
	uint32_t digital_toggle;

	//analog channels ordered according to the axis enumerator
	//range is from 0 to (1<<16), the zero state is (2<<15).
	int32_t analog[6];

	//getter for nice looking button reads
	int button(int i){
		return (digital & (1<<i)) != 0;
	};

	int button_toggle(int i){
		return (digital_toggle & (1<<i)) != 0;
	}

	float analogf(int channel){
		return (analog[channel]-(1<<15))/32768.0;
	}
	float analogfp(int channel){
		return analog[channel]/65536.0;
	}
	//construct and destruct
	Gamepad();
	Gamepad(const char* device);
	~Gamepad();

//-----------------------------------------------------------------------------
	//XXX everything from here on should be private
	//XXX but it has to be available for pthread to work
	//RAW DATA
	uint8_t raw[RAW_LENGTH];	//raw USB-HID data


	//these have to be public so that they can be called from within a thread
	int read_raw();
	int format();

private:	
	bool listening;
	//device and thread overhead
	int fd;						//device file descriptor
	pthread_t listener;			//listener thread ID
};

void* listen(void* arg);

#endif/*gamepad_h*/
