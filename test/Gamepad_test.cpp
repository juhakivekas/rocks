#include "../src/Gamepad.h"
#include <cstdio>

const char* DEVICE = "/dev/input/js0";

//test prototypes
int open();
int raw_read();
int thread_read();

int main(){
	int fail = 0;
	printf("---- Starting tests for Gamepad.\n");
	if(open() != 0) {printf("failed test open().\n"); fail++;}
//	if(raw_read() != 0) {printf("failed test read().\n"); fail++;}
	if(thread_read() != 0) {printf("failed test thread_read().\n"); fail++;}
	printf("---- Gamepad tests ran, %d failed.\n\n", fail);
	return 0;
}

//tests the constructor
int open(){
	Gamepad g = Gamepad(DEVICE);
	//Gamepad("/dev/input/js0");
	return 0;
}

//tests the single reading operation
int raw_read(){
	Gamepad g = Gamepad(DEVICE);
	int ret = g.read_raw();
	return ret;
}

int thread_read(){
	Gamepad g = Gamepad(DEVICE);
	printf("Let the thread run for some time...\n");
	usleep(500 * 1000);
	g.~Gamepad();
	return 0;
}
