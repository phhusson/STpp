#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Ax12.h>
#include <Exti.h>
#include <Uart.h>
#include <Usb.h>
#include <Watchdog.h>

extern "C" void vTaskDelay(int);
void capaInit(Gpio in, Gpio drive) {
	in
		.setDirection(Gpio::INPUT)
		.setResistor(Gpio::NONE)
		.setFunction(Gpio::GPIO);

	drive
		.setDirection(Gpio::INPUT)
		.setFunction(Gpio::GPIO)
		.setSpeed(Gpio::SPEED_100MHz);
}

int capaMeasure(Gpio in, Gpio drive) {
	in
		.setDirection(Gpio::INPUT)
		.setResistor(Gpio::NONE);

	drive
		.setDirection(Gpio::INPUT)
		.setResistor(Gpio::PULL_UP);
	vTaskDelay(2);

	drive
		.setResistor(Gpio::PULL_DOWN);

	Tim7
		.enable()
		.setCounter(0);
#if 1
	while(in);
	Tim7.disable();
#else
	while(Tim7.enabled());
#endif
	drive
		.setDirection(Gpio::OUTPUT)
		.setState(true);
	in
		.setDirection(Gpio::OUTPUT)
		.setState(true);
	return Tim7.getCounter();
}

int delta[2] = {1000000, 1000000};
int mean[2] = {0,0};
int mean2[2] = {0,0};
int max,min;
int main() {
	log << "startup" << Log::endl;

	Tim7
		.setPrescaler(0x0)
		.setAutoReload(65535)
		.setCounter(0x0)
		.setOneShot(true)
		.update();

	auto capa1_in = GpioD[2];
	auto capa1_drive = GpioD[0];

	auto capa2_in = GpioD[3];
	auto capa2_drive = GpioD[1];
	int i = 0;
	capaInit(capa1_in, capa1_drive);
	capaInit(capa2_in, capa2_drive);

	capaMeasure(capa1_in, capa1_drive);
	capaMeasure(capa2_in, capa2_drive);
	vTaskDelay(2);

	mean2[0] = 1024 * capaMeasure(capa1_in, capa1_drive);
	mean[0] = mean2[0];

	mean2[1] = 1024 * capaMeasure(capa2_in, capa2_drive);
	mean[1] = mean2[1];

#define FAST_SPEED 8 
#define SLOW_SPEED 4

	int d_mean[2] = {0, 0};
	while(1) {
		int v = capaMeasure(capa1_in, capa1_drive);
		mean2[0] = v + (SLOW_SPEED-1) * mean2[0] / SLOW_SPEED;
		v*=(SLOW_SPEED/FAST_SPEED);
		mean[0] = v + (FAST_SPEED-1) * mean[0] / FAST_SPEED;

		v = capaMeasure(capa2_in, capa2_drive);
		mean2[1] = v + (SLOW_SPEED - 1) * mean2[1] / SLOW_SPEED;
		v*=SLOW_SPEED/FAST_SPEED;
		mean[1] = v + (FAST_SPEED-1) * mean[1] / FAST_SPEED;

		int d[2];
		d[0] = mean[0] - mean2[0];
		d[1] = mean[1] - mean2[1];

		if( !(i%500))
			log << "Delta mean " << (mean2[0]-mean[0]) << Log::endl;
		++i;

		if(i == 2048) {
			log << "Starting properly !" << Log::endl;
			LedR.setDutyCycle(100);
		}

#if 0
		int abs_d[2];
		abs_d[0] = d[0] > 0 ? d[0] : -d[0];
		abs_d[1] = d[1] > 0 ? d[1] : -d[1];

		if(abs_d[0] > 50*abs_d[1] && abs_d[0] > 2000) {
			if(d[0]>0)
				LedB.setDutyCycle(100);
			else
				LedB.setDutyCycle(0);
		} else if(abs_d[1] > 50*abs_d[0] && abs_d[1] > 2000) {
			if(d[1]>0)
				LedO.setDutyCycle(100);
			else
				LedO.setDutyCycle(0);
		}
#endif
#if 1
		if(mean2[0] > delta[0]) {
			LedG.setDutyCycle(100);
		} else {
			LedG.setDutyCycle(0);
		}

#if 1
		if(mean2[1] > delta[1]) {
			LedO.setDutyCycle(100);
		} else {
			LedO.setDutyCycle(0);
		}
#endif
#endif


		vTaskDelay(2);
	}

	for(;;);
}
