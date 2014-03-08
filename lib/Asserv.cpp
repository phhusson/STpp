#include <Irq.h>
#include <Log.h>
#include "Asserv.h"

Asserv::Asserv(IncrementalEncoder& _left, IncrementalEncoder& _right,
	Timer& tim, HBridgeST mot1, HBridgeST mot2) :
	eLeft(_left), eRight(_right),
	tim(tim),
	motorl(mot1), motorr(mot2),
	c_propDist(600), c_propAngle(600),
	c_intDist(200), c_intAngle(200),
	intDist(0), intAngle(0),
	maxEngine(0x3ff), minEngine(0x30),
	targetAngle(0), targetDist(0),
	infos(left, right, eLeft, eRight) {
	tim
		.setPrescaler(42)
		.setAutoReload(1000)
		.setOneShot(false)
		.setUIE(true)
		.setURS(true);

	Irq(tim.irqNr())
		.setPriority(15)
		.enable();

	tim
		.setTopCB([this](int timer_id) {
			infos.compute(targetDist, targetAngle);

			int dl = 0, dr = 0;
			dl += c_propDist * infos.getDeltaDist();
			dr += c_propDist * infos.getDeltaDist();

			dl += c_intDist * infos.getIntegralDist();
			dr += c_intDist * infos.getIntegralDist();

			dl += c_propAngle * infos.getDeltaAngle();
			dr -= c_propAngle * infos.getDeltaAngle();

			dl += c_intAngle * infos.getIntegralAngle();
			dr -= c_intAngle * infos.getIntegralAngle();


			dl/=1024;
			dr/=1024;
			if(dl > maxEngine)
				dl=maxEngine;
			if(dl < -maxEngine)
				dl=-maxEngine;
			if(dr > maxEngine)
				dr=maxEngine;
			if(dr < -maxEngine)
				dr=-maxEngine;


			if(dr < minEngine && dr > 0)
				dr = 0;
			if(dr > -minEngine && dr < 0)
				dr = 0;

			if(dl < minEngine && dl > 0)
				dl = 0;
			if(dl > -minEngine && dl < 0)
				dl = 0;

			motorl.setSpeed(dl);
			motorr.setSpeed(dr);
		})
		.enable();
}

Asserv& Asserv::setTargetDist(int t) {
	intDist = 0;
	targetDist = t;
	return *this;
}

Asserv& Asserv::setTargetAngle(int t) {
	intAngle = 0;
	targetAngle = t;
	return *this;
}

Asserv& Asserv::setProportionnalDistance(int c) {
	c_propDist = c;
	return *this;
}

Asserv& Asserv::setProportionnalAngle(int c) {
	c_propAngle = c;
	return *this;
}

Asserv& Asserv::setIntegralDistance(int c) {
	c_intDist = c;
	return *this;
}

Asserv& Asserv::setIntegralAngle(int c) {
	c_intAngle = c;
	return *this;
}

Asserv& Asserv::setMaxEngine(int l) {
	maxEngine = l;
	return *this;
}

Asserv& Asserv::setMinEngine(int l) {
	minEngine = l;
	return *this;
}

Asserv& Asserv::reset() {
	tim.disable();
	motorl.setSpeed(0);
	motorr.setSpeed(0);
	return *this;
}

Asserv& Asserv::start() {
	left = right = 0;
	targetDist = targetAngle = 0;
	intDist = intAngle = 0;
	tim.enable();
	return *this;
}

int Asserv::getAngle() {
	return (left-right)/2;
}

int Asserv::getDist() {
	return (left+right)/2;
}
