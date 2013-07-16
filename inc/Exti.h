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
		void enableIRQ();
		void disableIRQ();
		void enableRising();
		void disableRising();
		void enableFalling();
		void disableFalling();
		void setTopCB();
		void setBottomCB();
};


#endif /* _EXTI_H */
