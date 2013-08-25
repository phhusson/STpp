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

	//Configure the pin
	auto ax12_pin = GpioB[10];
	ax12_pin
		.setPushPull()
		.setAlternate(Gpio::USART1_3)
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz)
		.setResistor(Gpio::PULL_UP);

	Ax12 ax12(Uart(3), 0xfe);
	ax12.setEndless();
	//Enable torque
	ax12.enable();

	while(1) {
		ax12.setSpeed(0x7ff);
		vTaskDelay(1000);
		ax12.setSpeed(0x3ff);
		vTaskDelay(1000);
	}
}
