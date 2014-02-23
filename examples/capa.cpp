#include <tr1/functional>

#include <Ax12.h>
#include <Board.h>
#include <Capa.h>
#include <Exti.h>
#include <Log.h>
#include <Mean.h>
#include <Uart.h>
#include <Usb.h>
#include <Watchdog.h>

extern "C" void vTaskDelay(int);
Capa capa;

int main() {
	log << "startup" << endl;

	capa
		.add(GpioD[0])
		.add(GpioD[1])
		.add(GpioD[2]);

	capa();

	Mean slow[3]={Mean(4), Mean(4), Mean(4)};
	Mean fast[3]={Mean(1), Mean(1), Mean(1)};

	LedO.setDutyCycle(0);
	LedG.setDutyCycle(0);
	LedB.setDutyCycle(0);
	LedR.setDutyCycle(0);
	int len[3] = {0, 0, 0};
	while(1) {

		capa();

		int i;
		for(i=0;i<3;++i) {
			slow[i]+=capa[i];
			fast[i]+=capa[i];
		}
		LedB.setDutyCycle(0);
		if(capa[1] > 14800)
			LedB.setDutyCycle(100);


		for(i=0;i<3;++i) {
			int d;
			d = fast[i] - slow[i];
			if(d>15) {
				switch(i) {
					case 0:
						LedG.setDutyCycle(100);
						break;
					case 1:
						//LedO.setDutyCycle(100);
						break;
					case 2:
						//LedR.setDutyCycle(100);
						break;
				};
			}
			if(d<-20) {
				++len[i];
				log << "Fast " << fast[i];
				log << " Slow " << slow[i];
				log << " Delta " << d;
				log << " Len " << len[i];
				log << " Instant " << capa[i] << endl;
				switch(i) {
					case 0:
						LedG.setDutyCycle(0);
						break;
					case 1:
						//LedO.setDutyCycle(0);
						break;
					case 2:
						LedR.setDutyCycle(0);
						break;
				};
			} else {
				if(len[i]) {
					log << "Fast " << fast[i];
					log << " Slow " << slow[i];
					log << " Delta " << d;
					log << " Len " << len[i];
					log << " Instant " << capa[i] << endl;
				}
				len[i] = 0;
			}
		}
	}

	for(;;);
}
