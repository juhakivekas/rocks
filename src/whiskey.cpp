#include "Beater.h"
#include "Bytebeat.h"
#include "Gamepad.h"

const char* device = "/dev/input/js0";
Gamepad g = Gamepad(device);

char beat_0(int t){
	return t>>(t>>((g.analog[RX]>>13))&4);//(g.analog[RX]>>14);
}

int main(){
	Beater JACK = Beater("beater");
	JACK.add_beat(beat_0, "1");
	JACK.activate();
	sleep(20);
}
