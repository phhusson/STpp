#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Ax12.h>
#include <Exti.h>
#include <Uart.h>
#include <Usb.h>
#include <Watchdog.h>

extern "C" void vTaskDelay(int);
int main() {
	log << "startup" << Log::endl;

	auto uart2_tx = GpioA[2];
	uart2_tx
		.setPushPull()
		.setAlternate(Gpio::USART1_3)
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz)
		.setResistor(Gpio::PULL_UP);

	vTaskDelay(100);

	Tim4
		.setPrescaler(42)
		.setAutoReload(1000)
		.enable();

	Ax12 ax12(Uart(2), 0x1);
	//ax12.setEndless();
	//ax12.setSpeed(0x7ff);
	//ax12.enable();
	Watchdog watcher(64, 0xfff);
	watcher
		.start()
		.stopOnDebug();

	int r = 0, b = 2, g = 4, o = 6;
	while(1) {
		watcher.reset();
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
