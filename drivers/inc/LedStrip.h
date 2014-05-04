#ifndef _LEDSTRIP_H
#define _LEDSTRIP_H
#include <Spi.h>

class LedStrip {
	private:
		Spi& spi;
	public:
		LedStrip(Spi& spi);
		LedStrip& push(int r, int g, int b);
		LedStrip& reset();
};

#endif /* _LEDSTRIP_H */
