#include "Time.h"
#include <unistd.h>

Time::Time(Timer& t) :
	timer(t) {
}

void Time::usleep(int t) {
	::usleep(t);
}

void Time::msleep(int t) {
	::usleep(t*1000);
}
