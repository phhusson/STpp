#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Debug.h>
#include <Exti.h>

extern "C" void vTaskDelay(int);
int main() {
	bool v = false;
			debug << "Hello !" << endl;
	Exti(UserButton)
		.enableFalling()
		.disableRising()
		.setTopCB([&](int) {
			debug << "Hello !" << endl;
			LedR.setDutyCycle(v ? 100 : 0);
			v = !v;
		})
		.enableIRQ();

	while(1)
		vTaskDelay(1000);
}
