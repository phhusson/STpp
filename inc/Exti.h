#ifndef _EXTI_H
#define _EXTI_H
#include "Gpio.h"

class Exti {
	public:
		enum Port {
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
		};
	private:
		void setGpioPort(int port);
		int chan;
		enum Port port;
		int irq_n();
	public:
		Exti(int c, Port p);
		Exti(const Gpio& p);
		Exti& enableIRQ();
		Exti& disableIRQ();
		Exti& enableRising();
		Exti& disableRising();
		Exti& enableFalling();
		Exti& disableFalling();
		Exti& setTopCB();
		Exti& setBottomCB();
};


#endif /* _EXTI_H */
