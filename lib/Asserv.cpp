#include <Irq.h>
#include <Log.h>
#include "Asserv.h"

Asserv::Asserv(IncrementalEncoder& left, IncrementalEncoder& right,
	Timer& tim, HBridgeST mot1, HBridgeST mot2) : left(left), right(right), tim(tim), motor1(mot1), motor2(mot2), ki(1) {
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
		.setTopCB([&left, &right, this](int timer_id) {
			left.update();
			right.update();
			int d1((angl+dist)/2 - left.getCounter()), d2((angl+dist)/2  - right.getCounter());
			d1 *= ki;
			d2 *= ki;
			d1 /= 10;
			d2 /= 10; 
			if(d1 > 1023)
				d1=1023;
			if(d1 < -1023)
				d1=-1023;
			if(d2 > 1023)
				d2=1023;
			if(d2 < -1023)
				d2=-1023;
			motor1.setSpeed(d1);
			motor2.setSpeed(d2);
		})
		.enable();
}

Asserv& Asserv::setTargetDist(int t){
	dist = t;
	return *this;
}

Asserv& Asserv::setAngleDist(int t){
	angl = t;
	return *this;
}

Asserv& Asserv::setPCorrector(int t){
	ki = t;
	return *this;
}
