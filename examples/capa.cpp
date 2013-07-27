#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Capa.h>
#include <Ax12.h>
#include <Exti.h>
#include <Uart.h>
#include <Usb.h>
#include <Watchdog.h>

extern "C" void vTaskDelay(int);
int main() {
	log << "startup" << Log::endl;

	Capa capa;
	capa
		.add(GpioD[0])
		.add(GpioD[1])
		.add(GpioD[2]);

	capa();
	int old[3];
	old[0] = capa[0];
	old[1] = capa[1];
	old[2] = capa[2];
	while(1) {
		capa();

		int i;

		LedO.setDutyCycle(0);
		LedG.setDutyCycle(0);
		LedB.setDutyCycle(0);
		LedR.setDutyCycle(0);
		for(i=0;i<3;++i) {
			int d;
			d = capa[i] - old[i];
			if(d>30) {
				switch(i) {
					case 0:
						LedG.setDutyCycle(100);
						break;
					case 1:
						LedO.setDutyCycle(100);
						break;
					case 2:
						LedR.setDutyCycle(100);
						break;
				};
			}
		}
		old[0] = capa[0];
		old[1] = capa[1];
		old[2] = capa[2];
	}

	for(;;);
}
