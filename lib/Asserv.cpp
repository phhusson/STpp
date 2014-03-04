#include <Irq.h>
#include <Log.h>
#include "Asserv.h"

Asserv::Asserv(IncrementalEncoder& left, IncrementalEncoder& right,
	Timer& tim, HBridgeST mot1, HBridgeST mot2) : left(left), right(right), tim(tim), motorl(mot1), motorr(mot2), kl(10), kr(10)  {
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
			int dl((angl+dist)/2), dr((dist-angl)/2);
			dl -= left.getCounter();
			dr -= right.getCounter();
			dl /= kl;
			dr /= kr;
			if(dl > 1023)
				dl=1023;
			if(dl < -1023)
				dl=-1023;
			if(dr > 1023)
				dr=1023;
			if(dr < -1023)
				dr=-1023;
			motorl.setSpeed(dl);
			motorr.setSpeed(dr);
		})
		.enable();
}

Asserv& Asserv::setTargetDist(int t){
	dist = t;
	return *this;
}

Asserv& Asserv::setTargetAngle(int t){
	angl = t;
	return *this;
}

Asserv& Asserv::setPCorrectors(int l, int r){
	kl = l;
	kr = r;
	return *this;
}
