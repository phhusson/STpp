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
	bool v = false;
	Exti(UserButton)
		.enableFalling()
		.disableRising()
		.setTopCB([&](int) {
			LedR.setDutyCycle(v ? 100 : 0);
			v = !v;
		})
		.enableIRQ();

	while(1)
		vTaskDelay(1000);
}
