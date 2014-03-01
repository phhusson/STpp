#include <Board.h>
#include "HBridgeST.h"
//#include <Log.h>

extern "C" void vTaskDelay(int);
int main() {
	//log << "startup" << Log::endl;

	HBridgeST motor0(Prop0A, Prop0B, Prop0_PWM, Tim2, 4);

	int v(0);

	while(1) {
		v+=100;
		v%= 1024;
		motor0.setSpeed(v);
		vTaskDelay(500);
	}
}
