#include "Uart.h"
#include <stm32f4xx.h>

Uart::Uart(int n) :
	number(n) {
	switch(n) {
		case 1:
			RCC->APB2ENR |= 1<<4;
			base = (volatile USART_TypeDef*)0x40011000;
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			RCC->APB1ENR |= 1<<(15+n);
			base = (volatile USART_TypeDef*)(0x40004400+(n-2)*0x100);
			break;
		case 6:
			RCC->APB2ENR |= 1<<5;
			base = (volatile USART_TypeDef*)0x40011400;
			break;
	}
}

Uart& Uart::configGpio(Gpio& p) {
	int af;

	if(number < 4)
		af = 7;
	else
		af = 8;

	p.setAlternate(af);
	p.setSpeed(Gpio::SPEED_100MHz);

	//Perhaps control direction... one day...
	return *this;
}

Uart& Uart::put(char c) {
	while(! (base->SR & (1<<7)));
	base->DR = c;
	return *this;
}

Uart& Uart::setMantissa(int m) {
	m&=0xfff;
	base->BRR = (base->BRR & ~(0xfff<<4)) | (m<<4);
	return *this;
}

Uart& Uart::setFraction(int f) {
	f&=0xf;
	base->BRR = (base->BRR & ~(0xf)) | f;
	return *this;
}

Uart& Uart::enableReceive() {
	base->CR1 |= 1<<2;
	return *this;
}

Uart& Uart::disableReceive() {
	base->CR1 &= ~(1<<2);
	return *this;
}

Uart& Uart::enableTransmitter() {
	base->CR1 |= 1<<3;
	return *this;
}

Uart& Uart::disableTransmitter() {
	base->CR1 &= ~(1<<3);
	return *this;
}

Uart& Uart::enable() {
	base->CR1 |= 1<<13;
	return *this;
}

Uart& Uart::disable() {
	base->CR1 &= ~(1<<13);
	return *this;
}

char Uart::waitForNext() {
	while( ! (base->SR & (1<<5)));
	return base->DR;
}
