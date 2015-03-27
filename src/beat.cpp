#include "Bytebeat.h"
#include "Beater.h"

char beat_0(int t){
//	return (
//		t>>((m[1]>>4)+(3&t>>7))	
//	);
	t &= (1<<17)-1;
	t += (5) * (1<<17);
	return (t&(t>>(7)))?0:0xff;
}

int main(){
	Beater JACK = Beater("Beater");
	JACK.add_beat(beat_0, "0");
	JACK.activate();
	sleep(60);
}
