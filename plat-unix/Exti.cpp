#include <stm32f4xx.h>
#include "Board.h"
#include "Exti.h"
#include "Irq.h"
#include "Log.h"

Exti::Callback Exti::top_cb[15];
Exti::Callback Exti::bottom_cb[15];

int Exti::irq_n() {
	return 0;
}

Exti& Exti::enableIRQ() {
	return *this;
}

Exti& Exti::disableIRQ() {
	return *this;
}

Exti& Exti::enableRising() {
	return *this;
}

Exti& Exti::disableRising() {
	return *this;
}

Exti& Exti::enableFalling() {
	return *this;
}

Exti& Exti::disableFalling() {
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
}


Exti::Exti(int c, Port p) :
	chan(c), port(p) {
}

Exti::Exti(const Gpio& p) {
}

void Exti::callTopCB(int nr) {
	top_cb[nr](nr);
}
