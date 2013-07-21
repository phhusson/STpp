#ifndef _UART_H
#define _UART_H

#include "Gpio.h"
#include <stm32f4xx.h>

class Uart {
	private:
		int number;
		volatile USART_TypeDef *base;
	public:
		Uart(int);
		Uart& configGpio(Gpio& p);
		char waitForNext();
		Uart& put(char);
		Uart& setMantissa(int);
		Uart& setFraction(int);
		Uart& enableReceive();
		Uart& disableReceive();
		Uart& enableTransmitter();
		Uart& disableTransmitter();
		Uart& enable();
		Uart& disable();
};

#endif /* _UART_H */
