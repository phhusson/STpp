#include <stm32f4xx.h>
#include "Gpio.h"

#define me ((volatile GPIO_TypeDef*)(this->port->base))

Gpio::Gpio(GpioPort *p, int n)
	: port(p), number(n) {
}

Gpio& Gpio::setState(bool v) {
	return *this;
}

bool Gpio::operator=(bool v) {
	return v;
}

Gpio& Gpio::setSpeed(Gpio::Speed s) {
	return *this;
}

Gpio& Gpio::setPushPull() {
	return *this;
}

Gpio& Gpio::setOpenDrain() {
	return *this;
}

Gpio& Gpio::setDirection(Gpio::Direction d) {
	return *this;
}

Gpio& Gpio::setFunction(Gpio::Function f) {
	return *this;
}

GpioPort::GpioPort(volatile GPIO_TypeDef* b) : base(b) {
}

Gpio& Gpio::setResistor(Resistor r) {
	return *this;
}

Gpio& Gpio::setAlternate(int af) {
	return *this;
}

Gpio& Gpio::setAlternate(AF a) {
	return *this;
}

int GpioPort::getPortNumber() {
	return 0;
}

Gpio& Gpio::operator=(Gpio& g) {
	return *this;
}

Gpio::Gpio() {
}
