#include "Beater.h"
#include "Context.h"

//scale to [0,7]
#define lx (d->lx>>5)
#define ly (d->ly)
#define rx (d->rx>>5)
#define ry (d->ry>>5)

char beat_0(int t, char_data *d){
	return ((t<<1&(0x100|(1<<lx)))?~t:t)^ly;
}

char beat_1(int t, char_data *d){
	return (t>>(ry+8)&1)?t<<(t>>(rx+8)&3):0;
}



int main(){
	Gamepad gamepad = Gamepad("/dev/input/js0");
	Context context = Context(&gamepad);
	Beater JACK = Beater(&context, "beater");
	JACK.add_beat(beat_0, "0");
	JACK.add_beat(beat_1, "1");
	//press button 10 to quit
	while(!gamepad.button(10));
}
