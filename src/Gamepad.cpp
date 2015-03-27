#include "Gamepad.h"

Gamepad::Gamepad(){
	listening = false;	
}

Gamepad::Gamepad(const char* device){
	//null everything
	digital = 0;
	time = 0;
	int i;
	for(i=0; i<6; i++) analog[i] = 0;

	//open device filedescriptor
	fd = open(device, O_RDONLY);
	if(fd == 0){
		perror("Error opening Gamepad device: ");
		printf("Cannot open %s, exiting.\n", device);
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
	fprintf(stderr, "Opened Gamepad device and started listener thread.\n");
}

Gamepad::~Gamepad(){
	if(listening){
		//kill listener thread
		pthread_cancel(listener);
		//close device filedescriptor
		close(fd);
		fprintf(stderr, "Closed Gamepad device and killed listener thread.\n");
	}
}

int Gamepad::read_raw(){
	//read the raw data
	//XXX is this endianness dependent?
	int status = read(fd, (void*)raw, RAW_LENGTH);
	if(status < 0){
		perror("Error reading Gamepad device: ");
		exit(1);
		//return 1;
	}
	//debug output
	//printf("Time: %02x%02x%02x%02x\n", raw[0], raw[1], raw[2], raw[3]);
	//printf("Data: %02x%02x%02x%02x\n", raw[4], raw[5], raw[6], raw[7]);
	
	return 0;
}

int Gamepad::format(){
	int analog_value, digital_value, channel, type;
	//PART 1: put things in neat boxes
	time =  raw[0] + (raw[1]<<8) + (raw[2]<<16) + (raw[3]<<24);

	analog_value = raw[4] + (raw[5]<<8);
	//flip a bit to make the value unsigned
	analog_value ^= (1<<15);
	
	digital_value = raw[4];

	type = raw[6];
	channel = raw[7];

	//PART 2: pack the boxes with red ribbon
	if(type == BUTTON || type == BUTTON_INIT){
		//make the channel correspond the labeled number of buttons
		channel += 1;
		//edit the bitmask of buttons
		if(digital_value == 1){
			digital |=  (1<<channel);
		}else{
			digital &= ~(1<<channel);
		}
	}else if(type == ANALOG || type == ANALOG_INIT){
		analog[channel] = analog_value;
	}else{
		fprintf(stderr, "Error in Gamepad data formatting\n");
		fprintf(stderr, "type:%d\n", type);
		fprintf(stderr, "chan:%d\n", channel);
		//something fucked up
		return 1;
	}
	return 0;
}

void* listen(void* arg){
	Gamepad *g = (Gamepad*) arg;
	//read the controller indefinetly, break if the read fails.
	while(g->read_raw() == 0){
		g->format();
		//place format prints here if you want to check something
		//printf("%10d %5d %5d\n", g->time, g->analog[LX], g->analog[LY]);	
		//printf("gamepad.lx:  %02x\n", g->analog[LX]);	
	}
	return NULL;
}
