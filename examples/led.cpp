#include <Board.h>
#include "Exti.h"

extern "C" void vTaskDelay(int);
int main() {
	Exti e(UserButton);
	UserButton.setDirection(Gpio::INPUT);
	UserButton.setResistor(Gpio::PULL_DOWN);

	e.enableRising();
	e.enableIRQ();
	Tim4.setPrescaler(42); // 100kHz
	Tim4.setAutoReload(1000); //1kHz
	Tim4.enable();

	LedG_USB.toggle();

	int r = 0, b = 2, g = 4, o = 6;
	while(1) {
		++r;
		++g;
		++b;
		++o;
		r%=10;
		g%=10;
		b%=10;
		o%=10;

		LedO.setComparator(1<<o);
		LedG.setComparator(1<<g);
		LedB.setComparator(1<<b);
		LedR.setComparator(1<<r);

		vTaskDelay(20);
	}
}
