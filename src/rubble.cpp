#include "Gamepad.h"

int main(){
	const char* device_path = "/dev/input/js0";
	Gamepad g = Gamepad(device_path);
	sleep(1);
	while(g.button(10) == 0){
		//100ms
		usleep(100* 1000);
		printf("%10d %6d %6d %03x\n", g.time, g.analog[RX], g.analog[RY], g.digital);
	}
	return 0;
}
