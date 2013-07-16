#include <stm32f4xx.h>
#include "Board.h"
#include "Exti.h"

void exti_handler(int v) {
}

void exti_handler() {
	int i = 0;
	unsigned long reg=EXTI->PR;
	while(reg) {
		if(reg&1) {
			exti_handler(i);
			EXTI->PR = 1<<i;
		}
		++i;
		reg>>=1;
	}
	LedG_USB.toggle();
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
	int irq;
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

void Exti::enableIRQ() {
	EXTI->IMR |= 1<<chan;
	int n = irq_n();
	NVIC->IP[n]=230;
	NVIC->ISER[ n >> 5] |= 1<<(n & 0x1f);
}

void Exti::disableIRQ() {
	EXTI->IMR &= ~(1<<chan);
}

void Exti::enableRising() {
	EXTI->RTSR |= 1<<chan;
}

void Exti::disableRising() {
	EXTI->RTSR &= ~(1<<chan);
}

void Exti::enableFalling() {
	EXTI->FTSR |= 1 << chan;
}

void Exti::disableFalling() {
	EXTI->FTSR &= ~(1<<chan);
}

void Exti::setTopCB() {
}

void Exti::setBottomCB() {
}

void Exti::setGpioPort(int port) {
	//SYSCFG_EXTICR1..4
	RCC->APB1ENR |= 1 << 14;
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
}
