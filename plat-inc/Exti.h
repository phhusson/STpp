#ifndef _EXTI_H
#define _EXTI_H
#include "Gpio.h"
#include <tr1/functional>

void exti_handler();
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
		typedef std::tr1::function<void(int)> Callback;
	private:
		void setGpioPort(int port);
		int chan;
		enum Port port;
		int irq_n();
		static Callback top_cb[15];
		static Callback bottom_cb[15];
		static void callTopCB(int nr);
		static void callBottomCB(int nr);
		friend void exti_handler();
	public:
		//typedef function<void(int)> Callback;
		Exti(int c, Port p);
		Exti(const Gpio& p);
		Exti& enableIRQ();
		Exti& disableIRQ();
		Exti& enableRising();
		Exti& disableRising();
		Exti& enableFalling();
		Exti& disableFalling();
		Exti& setTopCB(Callback cb);
		Exti& setBottomCB(Callback cb);
};


#endif /* _EXTI_H */
