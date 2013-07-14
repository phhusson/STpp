#include <stm32f4xx.h>
#include "Gpio.h"

#define me ((volatile GPIO_TypeDef*)(this->port->base))

Gpio::Gpio(GpioPort *p, int n)
	: port(p), number(n) {

	function = GPIO;
	direction = INPUT;
}

void Gpio::setState(bool v) {
	if(v) {
		//Set pin
		me->BSRRL = 1 << number;
	} else {
		//Reset pin
		me->BSRRH = 1 << number;
	}
}

bool Gpio::getState() {
	return !! (me->IDR & (1 << number));
}

void Gpio::setSpeed(Gpio::Speed s) {
	int v;
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
}

void Gpio::setPushPull() {
	me->OTYPER &= ~(1 << number);
}

void Gpio::setOpenDrain() {
	me->OTYPER |= 1 << number;
}

void Gpio::updateModeR() {
	if(direction==ANALOG)
		me->MODER = me->MODER | 3 << (2*number);
	else if(direction==INPUT)
		me->MODER = me->MODER & ~(3 << (2*number));
	else if(function != GPIO)
		me->MODER = (me->MODER & ~(3 << (2*number))) | 2 << (2*number);
	else
		me->MODER = (me->MODER & ~(3 << (2*number))) | 1 << (2*number);
}

void Gpio::setDirection(Gpio::Direction d) {
	direction = d;
	updateModeR();
}

void Gpio::setFunction(Gpio::Function f) {
	function = f;
	updateModeR();
}

GpioPort::GpioPort(volatile void* b) : base(b) {
	//Get the number of the GPIO port based on base
	int n = (((unsigned long)b) >> 10) & 0xf;
	RCC->AHB1ENR |= 1 << n;
}
