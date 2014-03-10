#include "HBridgeST.h"
#include "Log.h"

HBridgeST::HBridgeST(Gpio& a, Gpio& b, Gpio& p,
		Timer& tim, int chan) : propa(a), propb(b), pwm(p, tim, chan, 42, 1024){
	propa
		.setDirection(Gpio::OUTPUT)
		.setState(false);
	propb
		.setDirection(Gpio::OUTPUT)
		.setState(false);
}

void HBridgeST::setSpeed(int s){
	if (s<0) {
		propa.setState(true);
		propb.setState(false);
	} else {
		propa.setState(false);
		propb.setState(true);
	}
	pwm.setComparator((s<0) ? -s : s);
}

