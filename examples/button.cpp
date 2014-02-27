#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Debug.h>
#include <Exti.h>
#include <UsbSerial.h>

extern "C" void vTaskDelay(int);
int main() {
	bool v = false;
	UsbSerial usb;
	usb << "Hello !" << endl;
	Exti(UserButton)
		.enableFalling()
		.disableRising()
		.setTopCB([&](int) {
			usb << "Hello !" << endl;
			LedR.setDutyCycle(v ? 100 : 0);
			v = !v;
		})
		.enableIRQ();

	while(1)
		vTaskDelay(1000);
}
