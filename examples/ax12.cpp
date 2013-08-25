#include <Board.h>
#include <Log.h>
#include <Ax12.h>
#include <Uart.h>

extern "C" void vTaskDelay(int);
int main() {
	log << "startup" << Log::endl;

	//Configure the pin
	auto ax12_pin = GpioB[10];

	Ax12 ax12(ax12_pin, Uart(3), 0xfe);
	ax12.setEndless();
	//Enable torque
	ax12.enable();

	while(1) {
		ax12.setSpeed(0x7ff);
		vTaskDelay(200);
		ax12.setSpeed(0x3ff);
		vTaskDelay(200);
	}
}
