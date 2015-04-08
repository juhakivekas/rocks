#include "Beater.h"
#include "Context.h"

//scale to [0,7]
#define lx (d->lx>>6)
#define ly (d->ly>>6)
#define rx (d->rx)
#define ry (d->ry>>5)
#define r  (d->r)

unsigned char beat_0(int t, char_data *d){
	if(d->note == 0) return 0;
	return ((t<<1&(0x100|(1<<lx)))?~t:t);//^ly;
}

unsigned char beat_1(int t, char_data *d){
	if(d->note == 0) return 0;
	return (t>>(ry+8)&1)?t<<(t>>(rx+8)&3):0;
}

unsigned char beat_2(int t, char_data *d){
	if(d->note == 0) return 0;
	char s = r&(r>>2)&(lx+ly)?0:1;
	return s?t:t>>1;
}

unsigned char beat_3(int t, char_data *d){
	if(d->note == 0) return 0;
	int s = r;
	s &= (1<<5)-1;
	s += (ry) * (1<<5);
	return (s&(s>>(rx)))?0:t;
}

unsigned char beat_4(int t, char_data *d){
	if(d->note == 0) return 0;
	return t&1?0xff:0x00;
}

int main(){
	Gamepad gamepad = Gamepad("/dev/input/js0");
	//Gamepad gamepad = Gamepad();
	MIDIin midiin = MIDIin("/dev/snd/midiC1D0");
	BpmClock clock = BpmClock(60.0);
	Context context = Context(&gamepad, &clock, &midiin);
	Beater JACK = Beater(&context, "beater");
	JACK.add_beat(beat_0, "0");
	JACK.add_beat(beat_1, "1");
	//JACK.add_beat(beat_2, "2");
	//JACK.add_beat(beat_3, "3");
	//JACK.add_beat(beat_4, "4");
	//press button 10 to quit
	while(!gamepad.button(10));
}
