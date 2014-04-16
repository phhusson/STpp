#include "Spi.h"

void Spi::init() {
	if(n == 1) RCC->APB2ENR |= 1 << 12;
	else if(n == 2) RCC->APB1ENR |= 1 << 14;
	else if(n == 3) RCC->APB1ENR |= 1 << 15;
	else while(1);

	base = SPI1;
	base->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
}

Spi::Spi(int n, DmaStream* stream):
	n(n), dma(stream) {

	init();
	stream->
		peripheralFixed()
		.setDirection(DmaStream::M2P)
		.setPeripheral(&(base->DR));
}

Spi::Spi(int n): n(n), dma(0) {
	init();
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
	if(dma) {
		send(&v, 1);
		return *this;
	}
	while( !(base->SR & SPI_SR_TXE));
	base->DR=v;
	return *this;
}

Spi& Spi::send(char *s, int l) {
	if(!dma) {
		for(int i=0; i<l; ++i)
			send(s[i]);
		return *this;
	}

	txDma(true);
	dma->
		numberOfData(l)
		.setMemory(s)
		.enable()
		.wait();
	return *this;
}

Spi& Spi::setLsbFirst(bool lsbFirst) {
	base->CR1 |= SPI_CR1_LSBFIRST;
	return *this;
}

Spi& Spi::configGpio(Gpio& g) {
	g
		.setSpeed(Gpio::SPEED_100MHz);
	if(n == 1 || n == 2)
		g.setAlternate(Gpio::SPI1_2);
	else if(n==3)
		g.setAlternate(Gpio::SPI_3);
	return *this;
}

Spi& Spi::rxDma(bool enable) {
	base->CR2 |= SPI_CR2_RXDMAEN;
	return *this;
}

Spi& Spi::txDma(bool enable) {
	base->CR2 |= SPI_CR2_TXDMAEN;
	return *this;
}
