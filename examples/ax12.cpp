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
	//Enable torque
	ax12_broadcast.setSpeed(0x3ff);
	ax12_broadcast.enable();

	while(1) {
		ax12_broadcast.goTo(0x0);
		vTaskDelay(2000);
		ax12_broadcast.goTo(0x280);
		vTaskDelay(2000);
	}
}
