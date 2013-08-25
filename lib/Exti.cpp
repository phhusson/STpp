#include <stm32f4xx.h>
#include "Board.h"
#include "Exti.h"
#include "Irq.h"
#include "Log.h"

void exti_handler(int v) {
}

Exti::Callback Exti::top_cb[15];
Exti::Callback Exti::bottom_cb[15];
void *__dso_handle = 0;

void exti_handler() {
	int i = 0;
	unsigned long reg=EXTI->PR;
	while(reg) {
		if(reg&1) {
			Exti::callTopCB(i);
			exti_handler(i);
			EXTI->PR = 1<<i;
		}
		++i;
		reg>>=1;
	}
}

extern "C" {
	void EXTI0_IRQHandler() {
		exti_handler();
	}

	void EXTI1_IRQHandler() {
		exti_handler();
	}

	void EXTI2_IRQHandler() {
		exti_handler();
	}

	void EXTI3_IRQHandler() {
		exti_handler();
	}

	void EXTI4_IRQHandler() {
		exti_handler();
	}

	void EXTI9_5_IRQHandler() {
		exti_handler();
	}

	void EXTI15_10_IRQHandler() {
		exti_handler();
	}
};

int Exti::irq_n() {
	int irq = 40;
	switch(chan) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			irq = chan+6;
			break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			irq = 23;
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			irq = 40;
			break;
	};
	return irq;
}

Exti& Exti::enableIRQ() {
	EXTI->IMR |= 1<<chan;
	Irq(irq_n())
		.setPriority(230)
		.enable();
	return *this;
}

Exti& Exti::disableIRQ() {
	EXTI->IMR &= ~(1<<chan);
	Irq(irq_n())
		.disable();
	return *this;
}

Exti& Exti::enableRising() {
	EXTI->RTSR |= 1<<chan;
	return *this;
}

Exti& Exti::disableRising() {
	EXTI->RTSR &= ~(1<<chan);
	return *this;
}

Exti& Exti::enableFalling() {
	EXTI->FTSR |= 1 << chan;
	return *this;
}

Exti& Exti::disableFalling() {
	EXTI->FTSR &= ~(1<<chan);
	return *this;
}

Exti& Exti::setTopCB(Callback cb) {
	top_cb[chan] = cb;
	return *this;
}

Exti& Exti::setBottomCB(Callback cb) {
	bottom_cb[chan] = cb;
	return *this;
}

void Exti::setGpioPort(int port) {
	//SYSCFG_EXTICR1..4
	RCC->APB2ENR |= 1 << 14;
	volatile uint32_t *base = SYSCFG->EXTICR;
	base += chan/4;
	int c = chan%4;
	*base = (*base & ~(0xf << (c*4))) | (port << (c*4));
}


Exti::Exti(int c, Port p) :
	chan(c), port(p) {

	setGpioPort((int)p);
}

Exti::Exti(const Gpio& p) {
	chan = p.number;
	port = (Exti::Port)p.port->getPortNumber();
	setGpioPort((int)port);
}

void Exti::callTopCB(int nr) {
	top_cb[nr](nr);
}
