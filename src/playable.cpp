#include "Beater.h"
#include "Context.h"

//scale to [0,7]
#define lx (d->lx>>5)
#define ly (d->ly>>6)
#define rx (d->rx)
#define ry ((d->ry>>2)+0x40)
#define r  (d->r)

unsigned char beat_0(int t, char_data *d){
	if(d->note == 0) return 0;
	unsigned char tri = t&0x100?~t:t;
	if(tri < ry) tri = (ry<<1)-tri;
	return tri;
}

unsigned char beat_1(int t, char_data *d){
	if(d->note == 0) return 0;
	return t&(t>>lx)&((1<<(4-ly))-1);	
}

unsigned char beat_nomidi(int t, char_data *d){
	return t&(t>>8)?0x00:0xff;
}

int main(){
	Gamepad gamepad = Gamepad("/dev/input/js0");
	//Gamepad gamepad = Gamepad();
	//MIDIin midiin = MIDIin("/dev/snd/midiC1D0");
	MIDIin midiin = MIDIin();
	BpmClock clock = BpmClock(60.0);
	Context context = Context(&gamepad, &clock, &midiin);
	Beater JACK = Beater(&context, "beater");
	JACK.add_beat(beat_0, "0");
	JACK.add_beat(beat_1, "1");
	//JACK.add_beat(beat_nomidi, "nomidi");
	//press button 10 to quit
	while(!gamepad.button(10));
}
