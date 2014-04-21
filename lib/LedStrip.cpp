#include "LedStrip.h"

LedStrip::LedStrip(Spi& spi):
	spi(spi) {
	spi
		.master()
		.setLsbFirst(true)
		.setDivisorPow2(4)
		.enable();
}

static void ledSetBit(uint8_t *buf, int offset, int b) {
	buf += offset*3/8;
	int bit = (offset*3)%8;

	//Set first bit
	*buf |= 1 << bit;

	bit++;
	if(bit==8) { bit = 0; buf++; }

	if(b)
		*buf |= 1<<bit;
	else
		*buf &= ~(1<<bit);

	bit++;
	if(bit==8) { bit = 0; buf++; }

	*buf &= ~(1<<bit);
}

static void ledSetByte(uint8_t *buf, int offset, int v) {
	for(int i=0; i<8; ++i)
		ledSetBit(buf, offset+i, !!(v& (1<<(8-i))));
}

LedStrip& LedStrip::push(int r, int g, int b) {
	static uint8_t _dma_buf[9] __attribute__((section("dma")));
	ledSetByte(_dma_buf, 0, g);
	ledSetByte(_dma_buf, 8, r);
	ledSetByte(_dma_buf, 16, b);
	spi.send((char*)_dma_buf, 9);
	return *this;
}

LedStrip& LedStrip::reset() {
	static char _dma_buf[5] __attribute__((section("dma")));
	spi.send(_dma_buf, sizeof(_dma_buf));
	return *this;
}
