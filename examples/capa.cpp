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

	Tim7
		.setPrescaler(0)
		.setAutoReload(65535)
		.setCounter(0x0);

	auto tata = GpioD[1];
	int mean = 0;
	int i = 0;
	int max = 0;

	while(1) {
		tata
			.setResistor(Gpio::PULL_UP);
		vTaskDelay(2);

		Tim7.setCounter(0)
			.enable();
		tata.setResistor(Gpio::NONE);
		while(tata);
		Tim7.disable();
		int v = Tim7.getCounter();
		if(v< 0x1000 || v>0x80000)
			continue;
		mean = Tim7.getCounter() + 31 * mean / 32;
		if( !(i%30))
			log << "Mean = " << mean << ", Max=" << max << Log::endl;
		++i;
		if(mean > max)
			max = mean;
		if(mean < 0x118000) {
			LedG.setDutyCycle(100);
		} else {
			LedG.setDutyCycle(0);
		}

		vTaskDelay(10);
	}

	for(;;);
}
