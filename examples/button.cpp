#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Exti.h>

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
