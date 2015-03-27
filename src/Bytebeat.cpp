#include "Bytebeat.h"

Bytebeat::Bytebeat(bytebeat_func beat_in, const char* name_in){
	strncpy(name, name_in, 64);
	func = beat_in;
}

Bytebeat::~Bytebeat(){
	//DO NOTHING
}
