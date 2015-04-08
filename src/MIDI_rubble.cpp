#include "MIDIin.h"
#include <stdlib.h>
int main(){
	MIDIin M = MIDIin("/dev/snd/midiC1D0");
	sleep(20);
}
