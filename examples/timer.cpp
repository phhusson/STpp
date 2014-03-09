#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Ax12.h>
#include <Exti.h>
#include <Uart.h>
#include <Usb.h>
#include <Watchdog.h>

int main() {
	Tim4
		.setPrescaler(42)
		.setAutoReload(1000)
		.setOneShot(true)
		.enable();

	log << "Waiting for end of timer 4" << endl;
	while(Tim4);
	log << "Timer4 done" << endl;
	while(1)
		time.msleep(2000);
}
