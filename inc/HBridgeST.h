#ifndef _HBRIDGE_ST_H
#define _HBRIDGE_ST_H
#include <Gpio.h>
#include <Timer.h>
#include <Pwm.h>

class HBridgeST {
	private:
		Gpio propa;
		Gpio propb;
		Pwm pwm;
	public:
		HBridgeST(Gpio a, Gpio b, Gpio pwm, Timer tim, int chan);
		void setSpeed(int);
};

#endif /* _HBRIDGE_ST_H */
