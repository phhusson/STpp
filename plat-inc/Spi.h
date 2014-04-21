#ifndef _SPI_H
#define _SPI_H

#include "Gpio.h"
#include <stm32f4xx.h>
#include "OStream.h"
#include "IStream.h"
#include "Dma.h"

class Spi {
	private:
		int n;
		SPI_TypeDef *base;

		Spi& rxDma(bool enable=true);
		Spi& txDma(bool enable=true);
		DmaStream *dma;

		void init();
	public:
		Spi(int n);
		Spi(int n, DmaStream* s);
		Spi& enable(bool v=true);
		Spi& setDivisorPow2(int d);
		Spi& master();
		Spi& slave();
		Spi& setDataClockPhase(bool second);
		Spi& setClockPolarity(bool up);
		Spi& setLsbFirst(bool lsbFirst);
		Spi& send(char c);
		Spi& send(char* s, int len);
		Spi& configGpio(Gpio&);
};

#endif /* _SPI_H */
