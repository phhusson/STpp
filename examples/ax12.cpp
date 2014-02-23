#include <Board.h>
#include <Log.h>
#include <Ax12.h>
#include <Uart.h>

extern "C" void vTaskDelay(int);
int main() {
	log << "startup" << endl;

	//Configure the pin
	auto ax12_pin = GpioB[10];

	Ax12 ax12_broadcast(ax12_pin, Uart(3), 0xfe);
	ax12_broadcast.setEndless();
	//Enable torque
	ax12_broadcast.enable();

	while(1) {
		ax12_broadcast.setSpeed(0x7ff);
		vTaskDelay(200);
		ax12_broadcast.setSpeed(0x3ff);
		vTaskDelay(200);
	}
}
