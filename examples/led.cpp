#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include "Exti.h"
#include "Uart.h"
#include "Usb.h"

extern "C" void vTaskDelay(int);
int main() {
	log << "startup" << Log::endl;

	GpioA[2]
		.setPushPull()
		.setAlternate(Gpio::USART1_3)
		.setDirection(Gpio::OUTPUT)
		.setFunction(Gpio::ALTERNATE);

	GpioA[3]
		.setDirection(Gpio::INPUT)
		.setAlternate(Gpio::USART1_3)
		.setResistor(Gpio::PULL_DOWN);

	Uart uart2(2);
	uart2
		.setMantissa(13)
		.setFraction(0)
		.enableReceive()
		.enableTransmitter()
		.enable();

	Tim4
		.setPrescaler(42)
		.setAutoReload(1000)
		.enable();

	int r = 0, b = 2, g = 4, o = 6;
	while(1) {
		/*
		uart2.put('t');
		uart2.put('t');
		char c = uart2.waitForNext();
		for(;;);
		*/
		++r;
		++g;
		++b;
		++o;
		r%=10;
		g%=10;
		b%=10;
		o%=10;

		LedO.setComparator(1<<o);
		LedG.setComparator(1<<g);
		LedB.setComparator(1<<b);
		LedR.setComparator(1<<r);

		vTaskDelay(20);
	}
}
