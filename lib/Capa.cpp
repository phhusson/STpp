#include <Board.h>
#include <tr1/functional>
#include <Capa.h>
#include <Gpio.h>
#include <Log.h>
#include <Ax12.h>
#include <Exti.h>
#include <Uart.h>
#include <Usb.h>
#include <Watchdog.h>

extern "C" void vTaskDelay(int);
Capa::Capa() : n(0) {
	Tim7
		.setPrescaler(0x0)
		.setAutoReload(65535)
		.setCounter(0x0)
		.setOneShot(true)
		.update();
}

Capa& Capa::add(Gpio g) {
	int i = n;
	pins[n] = g;
	++n;

	Exti(g)
		.enableFalling()
		.setTopCB([=](int) {
			values[i] = Tim7.getCounter();
		})
		.enableIRQ();

	return *this;
}

void Capa::prepare(int i) {
	pins[i]
		.setFunction(Gpio::GPIO)
		.setDirection(Gpio::OUTPUT)
		.setState(true);
}

Capa& Capa::prepare() {
	int i;
	for(i=0;i<n;++i)
		prepare(i);

	return *this;
}

Capa& Capa::measure() {
	int i;
	Tim7
		.setCounter(0)
		.enable();

	//Set pins to input
	for(i=0;i<n;++i) {
		pins[i]
			.setDirection(Gpio::INPUT)
			.setResistor(Gpio::NONE);
		values[i] = 0;
		offset[i] = Tim7.getCounter();
	}

	while(Tim7.enabled()) {
		int i;
		int failed = 0;
		for(i=0;i<n;++i) {
			if(!values[i])
				failed = 1;
		}
		if(!failed)
			break;
	}
	if(!Tim7.enabled())
		log << "Timed out" << Log::endl;

	for(i=0;i<n;++i) {
		values[i]-=offset[i];
	}
	return *this;
}

Capa& Capa::update() {
	prepare();
	vTaskDelay(20);
	measure();
	return *this;
}
