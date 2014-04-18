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
	uint8_t buf[9];
	ledSetByte(buf, 0, g);
	ledSetByte(buf, 8, r);
	ledSetByte(buf, 16, b);
	spi.send((char*)buf, 9);
	return *this;
}

LedStrip& LedStrip::reset() {
	for(int i=0; i<900; ++i)
		spi.send(0);
	return *this;
}
