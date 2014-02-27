#ifndef _UART_H
#define _UART_H

#include "Gpio.h"
#include <stm32f4xx.h>
#include "OStream.h"
#include "IStream.h"

class Uart : public OStream, public IStream {
	private:
		int number;
		volatile USART_TypeDef *base;
	public:
		Uart(int);
		Uart& configGpio(Gpio& p);
		char waitForNext();
		Uart& setMantissa(int);
		Uart& setFraction(int);
		Uart& enableReceive();
		Uart& disableReceive();
		Uart& enableTransmitter();
		Uart& disableTransmitter();
		Uart& enable();
		Uart& disable();
		Uart& sendBreak();
		Uart& setHalfDuplex(bool);
	public:
		virtual Uart& put(char);
		virtual Uart& endl();
	public:
		virtual int get();
		virtual bool available();
};

#endif /* _UART_H */
