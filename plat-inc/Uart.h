#ifndef _UART_H
#define _UART_H

#include "Gpio.h"
#include <stm32f4xx.h>
#include "OStream.h"
#include "IStream.h"
#include "Dma.h"

class Uart : public OStream, public IStream {
	private:
		int number;
		volatile USART_TypeDef *base;
		void init();
		DmaStream* dma;
	public:
		Uart(int);
		Uart(int, DmaStream* dma);
		Uart& configGpio(Gpio& p);
		char waitForNext();
		Uart& setMantissa(int);
		Uart& setFraction(int);
		Uart& enableReceive();
		Uart& disableReceive();
		Uart& enableTransmitter();
		Uart& disableTransmitter();
		Uart& enable();
		Uart& txDma(bool en);
		Uart& disable();
		Uart& sendBreak();
		Uart& setHalfDuplex(bool);
	public:
		virtual Uart& put(char);
		virtual Uart& put(char*, int);
		virtual Uart& endl();
	public:
		virtual int get();
		virtual bool available();
		virtual void wait();
};

#endif /* _UART_H */
