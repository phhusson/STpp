#include "Spi.h"

Spi::Spi(int n) {
	base = I2S2ext_BASE + (SPI_TypeDef*)((n-1) * 0x400);
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
	return *this;
}

Spi& Spi::send(char v) {
	while( !(base->SR & SPI_SR_TXE));
	base->DR=v;
	while( !(base->SR & SPI_SR_TXE));
	return *this;
}
