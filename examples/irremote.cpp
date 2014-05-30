#include <Board.h>
#include <Bluetooth.h>
#include <Gpio.h>
#include <Exti.h>
#include <Irq.h>
#include <Tasks.h>
#include <Timer.h>
#include <Log.h>
#include <Lock.h>
#include <IRRemote.h>
#include <Debug.h>

int main() {
	auto pin1 = GpioE[7];
	IRRemote remote(Tim12, pin1);
	while(true)
		debug << remote.next();
}
