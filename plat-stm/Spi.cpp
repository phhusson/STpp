#include "Spi.h"

Spi::Spi(int n) {
	if(n == 1) RCC->APB2ENR |= 1 << 12;
	else if(n == 2) RCC->APB1ENR |= 1 << 14;
	else if(n == 3) RCC->APB1ENR |= 1 << 15;
	else while(1);

	base = SPI1;
	base->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
}

Spi& Spi::enable() {
	base->CR1 |= SPI_CR1_SPE;
	return *this;
}

Spi& Spi::master() {
	base->CR1 |= SPI_CR1_MSTR;
	return *this;
}

Spi& Spi::setDivisorPow2(int v) {
	base->CR1 &= ~SPI_CR1_BR;
	v &= 0x7;
	base->CR1 |= v << 3;
	return *this;
}

Spi& Spi::send(char v) {
	while( !(base->SR & SPI_SR_TXE));
	base->DR=v;
	while( !(base->SR & SPI_SR_TXE));
	return *this;
}
