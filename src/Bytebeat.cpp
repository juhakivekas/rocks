#include "Bytebeat.h"

Bytebeat::Bytebeat(const char* name_in, bytebeat_func beat_in){
	strncpy(name, name_in, 64);
	beat = beat_in;
}

Bytebeat::~Bytebeat(){
	//DO NOTHING
}
