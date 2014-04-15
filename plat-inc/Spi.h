#ifndef _SPI_H
#define _SPI_H

#include "Gpio.h"
#include <stm32f4xx.h>
#include "OStream.h"
#include "IStream.h"

class Spi {
	private:
		int n;
		SPI_TypeDef *base;
	public:
		Spi(int n);
		Spi& enable();
		Spi& setDivisorPow2(int d);
		Spi& master();
		Spi& slave();
		Spi& setDataClockPhase(bool second);
		Spi& setClockPolarity(bool up);
		Spi& setEndian(bool le);
		Spi& send(char c);
};

#endif /* _SPI_H */
