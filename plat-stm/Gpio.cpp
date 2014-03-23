#include <stm32f4xx.h>
#include "Gpio.h"

#define me ((volatile GPIO_TypeDef*)(this->port->base))

Gpio::Gpio(GpioPort *p, int n)
	: port(p), number(n) {
	function = GPIO;
	direction = INPUT;
}

Gpio& Gpio::setState(bool v) {
	if(v) {
		//Set pin
		me->BSRRL = 1 << number;
	} else {
		//Reset pin
		me->BSRRH = 1 << number;
	}
	return *this;
}

bool Gpio::operator=(bool v) {
	setState(v);
	return v;
}

#if 0
bool Gpio::getState() {
	return !! (me->IDR & (1 << number));
}
#endif

Gpio& Gpio::setSpeed(Gpio::Speed s) {
	int v = 0;
	switch(s) {
		case SPEED_2MHz:
			v = 0;
			break;
		case SPEED_25MHz:
			v = 1;
			break;
		case SPEED_50MHz:
			v = 2;
			break;
		case SPEED_100MHz:
			v = 3;
			break;
	};
	me->OSPEEDR = (me->OSPEEDR & ~(3 << (2*number))) | v << (2*number);
	return *this;
}

Gpio& Gpio::setPushPull() {
	me->OTYPER &= ~(1 << number);
	return *this;
}

Gpio& Gpio::setOpenDrain() {
	me->OTYPER |= 1 << number;
	return *this;
}

void Gpio::updateModeR() {
	if(direction==ANALOG)
		me->MODER = me->MODER | 3 << (2*number);
	else if(function != GPIO)
		me->MODER = (me->MODER & ~(3 << (2*number))) | 2 << (2*number);
	else if(direction==INPUT)
		me->MODER = me->MODER & ~(3 << (2*number));
	else
		me->MODER = (me->MODER & ~(3 << (2*number))) | 1 << (2*number);
}

Gpio& Gpio::setDirection(Gpio::Direction d) {
	direction = d;
	updateModeR();
	return *this;
}

Gpio& Gpio::setFunction(Gpio::Function f) {
	function = f;
	updateModeR();
	return *this;
}

GpioPort::GpioPort(volatile GPIO_TypeDef* b) : base(b) {
	//Get the number of the GPIO port based on base
	int n = getPortNumber();
	RCC->AHB1ENR |= 1 << n;
}

Gpio& Gpio::setResistor(Resistor r) {
	int v = 0;
	switch(r) {
		case NONE:
			v=0;
			break;
		case PULL_UP:
			v=1;
			break;
		case PULL_DOWN:
			v=2;
			break;
	};
	me->PUPDR = (me->PUPDR & ~(3<<(2*number))) | (v<<(2*number));
	return *this;
}

Gpio& Gpio::setAlternate(int af) {
	setFunction(ALTERNATE);
	int n = number;
	if(n>=8) {
		n-=8;
		me->AFR[1] = (me->AFR[1] & ~(0xf<<(4*n))) |
			af << (4*n);
	} else {
		me->AFR[0] = (me->AFR[0] & ~(0xf<<(4*n))) |
			af << (4*n);
	}
	return *this;
}

Gpio& Gpio::setAlternate(AF a) {
	setAlternate((int)a);
	return *this;
}

int GpioPort::getPortNumber() {
	int n = (((unsigned long)base) >> 10) & 0xf;
	return n;
}

Gpio& Gpio::operator=(Gpio& g) {
	number = g.number;
	port = g.port;
	function = g.function;
	direction = g.direction;

	return *this;
}

Gpio::Gpio() {
	number = -1;
}
