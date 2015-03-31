#ifndef clock_h
#define clock_h

#include <pthread.h>
#include <cstdio>
#include <cstdint>
#include <stdlib.h>
#include <unistd.h>

class BpmClock{
public:
	//this stores the number of pulses recieved since construction
	unsigned int pulse;
	float bpm;
	
	BpmClock(float bpm_in);
	~BpmClock();
	float set_bpm(float bpm_in);
	unsigned int beat_micro;
private:
	pthread_t thread_id;
};

void* RTC_counter(void* arg);
#endif/*clock_h*/
