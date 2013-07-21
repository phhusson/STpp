#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include "Exti.h"
#include "Uart.h"
#include "Usb.h"

extern "C" void vTaskDelay(int);
extern "C" void USB_OTG_BSP_mDelay(uint32_t);
int main() {
	log << "startup" << Log::endl;

	auto uart2_tx = GpioA[2];
	auto uart2_rx = GpioA[3];
	uart2_tx
		.setPushPull()
		.setAlternate(Gpio::USART1_3)
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz);

	uart2_rx
		.setDirection(Gpio::INPUT)
		.setAlternate(Gpio::USART1_3)
		.setResistor(Gpio::PULL_DOWN);

	Uart uart2(2);
	uart2
		.enable()
		.setMantissa(234)
		.setFraction(6)
		.enableReceive()
		.enableTransmitter();

	Tim4
		.setPrescaler(42)
		.setAutoReload(1000)
		.enable();

	int r = 0, b = 2, g = 4, o = 6;
	while(1) {
#if 1
		uart2
			.put('t')
			.put('t')
			.put('t')
			.put('t')
			.put('t')
			.put('t')
			.put('t')
			.put('t');
		char c = uart2.waitForNext();
		for(;;);
#endif

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
