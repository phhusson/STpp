#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Exti.h>
#include <Uart.h>
#include <Usb.h>
#include <Watchdog.h>
#include <IncrementalEncoder.h> 

int main() {
	IncrementalEncoder codeuse1(GpioE[9], GpioE[11], Tim1);

	while(1){
		time.msleep(200);
		log << "Counter value : " << codeuse1 << Log::endl;
	}

}
