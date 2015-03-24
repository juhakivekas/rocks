#include "Jack_beater.h"
#include "Bytebeat.h"
#include "Gamepad.h"

const char* device = "/dev/input/js0";
Gamepad g = Gamepad(device);

char beat_0(int t){
//	return (
//		t>>((m[1]>>4)+(3&t>>7))	
//	);
	t &= (1<<17)-1;
	t += (g.analog[RX]>>12) * (1<<17);
	return (t&(t>>(g.analog[RY]>>12)))?0:0xff;
}

int main(){
	Jack_beater JACK = Jack_beater("beater");
	Bytebeat b = Bytebeat("one", beat_0);
	JACK.add_beat(&b);
	JACK.activate();
	sleep(60);
}
