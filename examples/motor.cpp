#include <Board.h>
#include <Log.h>

extern "C" void vTaskDelay(int);
int main() {
	log << "startup" << Log::endl;

	Tim2
		.setPrescaler(42)
		.setAutoReload(1024) // change name of the function
		.setOneShot(false)
		.enable();

	int v(0);

	Prop0A
		.setDirection(Gpio::OUTPUT)
		.setState(true);

	Prop0B
		.setDirection(Gpio::OUTPUT)
		.setState(false);

	while(1) {
		v+=100;
		v%= 1024;
		Prop0_PWM.setComparator(v);
		vTaskDelay(200);
	}
}
