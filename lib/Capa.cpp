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
		.setAutoReload(20000)
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
			prepare(i);
		})
		.enableIRQ();

	return *this;
}

void Capa::prepare(int i) {
	pins[i]
		.setFunction(Gpio::GPIO)
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz)
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
		.setOneShot(true)
		.update()
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

	Tim7
		.setCounter(0)
		.disable();
	for(i=0;i<n;++i) {
		values[i]-=offset[i];
	}
	return *this;
}

extern "C" void USB_OTG_BSP_uDelay(int);
extern "C" void USB_OTG_BSP_mDelay(int);
Capa& Capa::update() {
	prepare();
	//USB_OTG_BSP_mDelay(2);
	vTaskDelay(10);
	measure();
	return *this;
}
