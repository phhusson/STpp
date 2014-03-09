#include "Asserv.h"

VelocityAccelPerEncoderCompute::VelocityAccelPerEncoderCompute() {
	*this = 0;
}

void VelocityAccelPerEncoderCompute::compute(int v) {
	int velocity = v - last;

	lastAccel = velocity - lastVelocity;
	lastVelocity = velocity;
	last = v;
}

int VelocityAccelPerEncoderCompute::getVelocity() {
	return lastVelocity;
}

int VelocityAccelPerEncoderCompute::getAccel() {
	return lastAccel;
}

int VelocityAccelPerEncoderCompute::getCurrent() {
	return last;
}

VelocityAccelPerEncoderCompute::operator int() {
	return last;
}

void VelocityAccelPerEncoderCompute::operator()(int v) {
	compute(v);
}

int VelocityAccelPerEncoderCompute::operator=(int v) {
	if(v != 0) while(1);
	last = lastVelocity = lastAccel = 0;
	return v;
}

VelocityAccel::VelocityAccel(VelocityAccelPerEncoderCompute &left, VelocityAccelPerEncoderCompute& right,
	IncrementalEncoder& eLeft, IncrementalEncoder& eRight, int N, int D) :
	left(left), right(right),
	eLeft(eLeft), eRight(eRight),
	intDist(0), intAngle(0),
	targetDist(0), targetAngle(0),
	N(N), D(D) {
}

void VelocityAccel::compute(int targetDist, int targetAngle) {
	eLeft.update();
	eRight.update();

	left.compute(eLeft);
	right.compute( (N*eRight)/D );

	if(this->targetDist != targetDist)
		intDist = 0;
	this->targetDist = targetDist;

	if(this->targetAngle != targetAngle)
		intAngle = 0;
	this->targetAngle = targetAngle;
}

int VelocityAccel::getIntegralAngle() {
	intAngle = getDeltaAngle()/16 + intAngle*15/16;
	return intAngle;
}

int VelocityAccel::getIntegralDist() {
	intDist = getDeltaDist()/16 + intDist*15/16;
	return intDist;
}

int VelocityAccel::getAngle() {
	return (left-right)/2;
}

int VelocityAccel::getDist() {
	return (left+right)/2;
}

int VelocityAccel::getDeltaAngle() {
	return targetAngle - getAngle();
}

int VelocityAccel::getDeltaDist() {
	return targetDist - getDist();
}

int VelocityAccel::getVelocityAngle() {
	return (left.getVelocity() - right.getVelocity())/2;
}

int VelocityAccel::getVelocityDist() {
	return (left.getVelocity() + right.getVelocity())/2;
}

int VelocityAccel::getAccelerationAngle() {
	return (left.getAccel() - right.getAccel())/2;
}

int VelocityAccel::getAccelerationDist() {
	return (left.getAccel() + right.getAccel())/2;
}
