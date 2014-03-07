#include "Asserv.h"

VelocityAccelCompute::VelocityAccelCompute() {
	*this = 0;
}

void VelocityAccelCompute::compute(int v) {
	lastInt = v/16 + (15 * lastInt) / 16;

	int velocity = v - last;

	lastAccel = velocity - lastVelocity;
	lastVelocity = velocity;
	last = v;
}

int VelocityAccelCompute::getIntegral() {
	return lastInt;
}

int VelocityAccelCompute::getVelocity() {
	return lastVelocity;
}

int VelocityAccelCompute::getAccel() {
	return lastAccel;
}

int VelocityAccelCompute::getCurrent() {
	return last;
}

VelocityAccelCompute::operator int() {
	return last;
}

void VelocityAccelCompute::operator()(int v) {
	compute(v);
}

int VelocityAccelCompute::operator=(int v) {
	if(v != 0) while(1);
	last = lastInt = lastVelocity = lastAccel = 0;
	return v;
}
