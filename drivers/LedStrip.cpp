#include "LedStrip.h"
#include <Log.h>

LedStrip::LedStrip(Spi& spi):
	spi(spi) {
	spi
		.master()
		.setLsbFirst(true)
		.setDivisorPow2(4)
		.enable();
}

static void ledSetBit(uint8_t *buf, int offset, int b) {
	int o = (offset*3)+1;
	buf += o/8;
	int bit = o%8;

	if(b)
		*buf |= 1<<bit;
	else
		*buf &= ~(1<<bit);
}

static void ledSetByte(uint8_t *buf, int offset, int v) {
	v&=0xff;
	for(int i=0; i<8; ++i) {
		ledSetBit(buf, offset+i, !!(v& (1<<(7-i))));
	}
}

static void ledstrip_init(uint8_t *buf) {
	for(int i=0; i<24; ++i) {
		uint8_t *b = buf + i*3/8;
		int bit = (i*3)%8;
		//Set bit 0 of bBit
		*b |= 1 << bit;

		bit += 2;
		if(bit >= 8) {
			bit -= 8; b++;
		}

		//Clear bit 2 of bBit
		*b &= ~(1<<bit);
	}
}

LedStrip& LedStrip::init(uint8_t *buf, int nleds, int len) {
	if(len < nleds*9) while(1);
	for(int i=0; i<nleds; ++i)
		ledstrip_init(buf+9*i);

	return *this;
}

LedStrip& LedStrip::prepare(uint8_t *buf, int id, int r, int g, int b) {
	ledSetByte(buf+id*9, 0, g);
	ledSetByte(buf+id*9, 8, r);
	ledSetByte(buf+id*9, 16, b);
	return *this;
}

LedStrip& LedStrip::push(int r, int g, int b) {
	static uint8_t _dma_buf[9] __attribute__((section("dma")));
	static int inited = 0;
	if(!inited) {
		inited = 1;
	}
	ledstrip_init(_dma_buf);
	(void) r;
	(void) g;
	(void) b;
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

LedStrip& LedStrip::push(uint8_t *buf, int nleds) {
	spi.send((char*)buf, nleds*9);
	reset();
	return *this;
}
