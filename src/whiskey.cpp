#include "Beater.h"
#include "Context.h"

//scale to [0,7]
#define rx (d->rx>>5)

char beat_0(int t, char_data *d){
	//return t>>(t>>(rx+5)&7);
	return t>>2;
}

int main(){
	Gamepad gamepad = Gamepad("/dev/input/js0");
	Context context = Context(&gamepad);
	Beater JACK = Beater(&context, "beater");
	JACK.add_beat(beat_0, "0");
	JACK.activate();
	sleep(20);
}
