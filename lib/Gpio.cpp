#include <stm32f4xx.h>
#include "Gpio.h"

//DM00031020.pdf p53
GpioPort GpioA(GPIOA);
GpioPort GpioB(GPIOB);
GpioPort GpioC(GPIOC);
GpioPort GpioD(GPIOD);
GpioPort GpioE(GPIOE);
GpioPort GpioF(GPIOF);
GpioPort GpioG(GPIOG);
GpioPort GpioH(GPIOH);
GpioPort GpioI(GPIOI);

#define me ((volatile GPIO_TypeDef*)(this->port->base))

Gpio::Gpio(GpioPort *p, int n)
	: port(p), number(n) {
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
