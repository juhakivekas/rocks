#include "Bytebeat.h"
#include "Jack_beater.h"

char beat_0(int t){
//	return (
//		t>>((m[1]>>4)+(3&t>>7))	
//	);
	t &= (1<<17)-1;
	t += (5) * (1<<17);
	return (t&(t>>(7)))?0:0xff;
}

int main(){
	Jack_beater JACK = Jack_beater("beater");
	Bytebeat b = Bytebeat("one", beat_0);
	JACK.add_beat(&b);
	JACK.activate();
	sleep(60);
}
