#ifndef _PWM_H
#define _PWM_H
#include <Gpio.h>
#include <Timer.h>

class Pwm {
	private:
		Gpio gpio;
		Timer tim;
		int chan;
	public:
		Pwm(Gpio p, Timer t, int n);
		Pwm& setDutyCycle(float v);
		Pwm& setDutyCycle(int percent);
		Pwm& setComparator(unsigned int cmp);
};
#endif /* _PWM_H */
