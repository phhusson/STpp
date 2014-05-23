#ifndef _LEDSTRIP_H
#define _LEDSTRIP_H
#include <Spi.h>

class LedStrip {
	private:
		Spi& spi;
	public:
		LedStrip(Spi& spi);
		LedStrip& init(uint8_t *buf, int nleds, int len);
		LedStrip& prepare(uint8_t *buf, int id, int r, int g, int b);
		LedStrip& push(uint8_t *buf, int nleds);
		LedStrip& push(int r, int g, int b);
		LedStrip& reset();
};

#endif /* _LEDSTRIP_H */
