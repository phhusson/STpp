#include "Debug.h"
#include "Time.h"

Time::Time(Timer& t) :
	timer(t) {

	timer
		.setPrescaler(168/2)
		.setOneShot(true);
}

void Time::usleep(int t) {
	timer
		.setAutoReload(t)
		.enable();
	while(timer);
}

extern "C" void vTaskDelay(int);
void Time::msleep(int t) {
	vTaskDelay(t);
}
