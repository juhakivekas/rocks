#include "BpmClock.h"

BpmClock::BpmClock(float bpm_in){
	set_bpm(bpm_in);
	pulse = 0;
	int status = pthread_create(
		&thread_id,
		NULL,
		RTC_counter,
		(void*)this
	);
	if(status != 0){
		perror("Error starting BpmClock thread");
		exit(1);
	}
	fprintf(stderr, "Started realtime master clock.\n");
}

BpmClock::~BpmClock(){
	pthread_cancel(thread_id);
}

float BpmClock::set_bpm(float bpm_in){
	float division = 24.0;
	bpm_in *= division;
	beat_micro = (unsigned int)((1000000.0*60.0)/bpm_in);
	//there might be a small error in the bpm
	bpm = 60.0*1000000.0/(float) beat_micro;
	fprintf(stderr, "BPM argument: %f\nActual BPM:   %f\n",
		bpm_in/division, bpm/division);
	return bpm;
}

void* RTC_counter(void* arg){
	BpmClock *c = (BpmClock*) arg;
	while(1){
		usleep(c->beat_micro);
		c->pulse++;
	}
	return NULL;
}
