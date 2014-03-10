#include <Irq.h>
#include <Log.h>
#include "Asserv.h"

Asserv::Asserv(IncrementalEncoder& _left, IncrementalEncoder& _right,
	Timer& tim, HBridgeST mot1, HBridgeST mot2) :
	eLeft(_left), eRight(_right),
	tim(tim),
	motorl(mot1), motorr(mot2),
	c_propDist(0x1000), c_propAngle(0x80),
	c_intDist(0xa), c_intAngle(0x3),
	c_velDist(0x80), c_velAngle(0),
	c_accelDist(0x10), c_accelAngle(0),
	maxEngine(0x3ff), minEngine(0x80),
	targetAngle(0), targetDist(0),
	infos(left, right, eLeft, eRight, 1, 1),
	maxAccel(0x80), waiting(false) {
	tim
		.setPrescaler(42)
		.setAutoReload(1000)
		.setOneShot(false)
		.setUIE(true)
		.setURS(true);

	int throttle = 100;

	Irq(tim.irqNr())
		.setPriority(15)
		.enable();

	tim
		.setTopCB([&tim, this](int timer_id) {
			infos.compute(targetDist, targetAngle);

			int d_d = 0, d_a = 0;
			//Distance
			d_d += c_accelDist * infos.getAccelDist();
			d_d += c_velDist * infos.getVelocityDist();
			d_d += c_propDist * infos.getDeltaDist();
			d_d += c_intDist * infos.getIntegralDist();

			//Angle
			d_a += c_accelAngle * infos.getAccelDist();
			d_a += c_velAngle * infos.getVelocityAngle();
			d_a += c_propAngle * infos.getDeltaAngle();
			d_a += c_intAngle * infos.getIntegralAngle();

#define abs(x) ((x) > 0 ? (x) : -(x))
#define signof(x, y) ((x) > 0 ? (y) : -(y))

			int dl = d_d + d_a;
			int dr = d_d - d_a;

			dl/=0x4000;
			dr/=0x4000;

			if(abs(dl) > maxEngine || abs(dr) > maxEngine) {
				if(abs(dl) > abs(dr)) {
					dr = signof(dr, abs(dr)-abs(dl)+maxEngine);
					dl = signof(dl, maxEngine);
				} else {
					dl = signof(dl, abs(dl)-abs(dr)+maxEngine);
					dr = signof(dr, maxEngine);
				}
			}

			int tot = abs(dl)+abs(dr);

			if(tot < minEngine) {
				dl = 0;
				dr = 0;
			}

			//Check we're not blocked
			if(infos.getVelocityDist() == 0 && infos.getVelocityAngle() == 0)
				beenZero++;
			else
				beenZero = 0;

			//150ms
			if(beenZero > 150) {
				dl = 0;
				dr = 0;
			}

			//ABS/ESP
			if(infos.getAccelDist() > maxAccel || infos.getAccelDist() < -maxAccel) {
				log << "Got max acceleration ! " << endl;
				must_throttle = true;
				if(throttle > 0) {
					throttle -= 10;
				}
			} else {
				if(throttle < 100)
					throttle += 10;
			}
			int ref = 0;
			if( (infos.getAccelDist() * infos.getVelocityDist()) > 0) {
				//We are accelerating
				//Throttling means reducing power
				ref = 0;
			} else if( (infos.getAccelDist() * infos.getVelocityDist()) < 0) {
				//We are braking
				//Throttling means increasing power
				ref = 200;
			}

			//We're not moving, and we're not telling the motors to move
			//Sounds ok ?
			waiting = (dl == 0) && (dr == 0) &&
				(infos.getVelocityDist() == 0) && (infos.getVelocityAngle() == 0);

			motorl.setSpeed( (ret*dl)/100 );
			motorr.setSpeed( (ret*dr)/100 );
		})
		.enable();
}

Asserv& Asserv::setTargetDist(int t) {
	beenZero = 0;
	targetDist = t;
	return *this;
}

Asserv& Asserv::setTargetAngle(int t) {
	beenZero = 0;
	targetAngle = t;
	return *this;
}

Asserv& Asserv::setProportionnalDist(int c) {
	c_propDist = c;
	return *this;
}

Asserv& Asserv::setProportionnalAngle(int c) {
	c_propAngle = c;
	return *this;
}

Asserv& Asserv::setIntegralDist(int c) {
	c_intDist = c;
	return *this;
}

Asserv& Asserv::setIntegralAngle(int c) {
	c_intAngle = c;
	return *this;
}

Asserv& Asserv::setVelocityAngle(int c) {
	c_velAngle = c;
	return *this;
}

Asserv& Asserv::setVelocityDist(int c) {
	c_velDist = c;
	return *this;
}

Asserv& Asserv::setAccelDist(int c) {
	c_accelDist = c;
	return *this;
}

Asserv& Asserv::setAccelAngle(int c) {
	c_accelAngle = c;
	return *this;
}

Asserv& Asserv::setMaxAcceleration(int l) {
	maxAccel = l;
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
	tim.enable();
	return *this;
}

int Asserv::getAngle() {
	return (left-right)/2;
}

int Asserv::getDist() {
	return (left+right)/2;
}

Asserv& Asserv::angle(int a) {
	setTargetAngle(targetAngle+a);
	return *this;
}

Asserv& Asserv::dist(int t) {
	setTargetDist(targetDist+t);
	return *this;
}
