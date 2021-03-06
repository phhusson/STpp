#include <Board.h>
#include <Log.h>
#include <Ax12.h>
#include <Uart.h>

int main() {
	log << "startup" << endl;

	//DmaStream uartDma(1, 4, 7);
	//Configure the pin
	//auto ax12_pin = GpioB[10];
	//Ax12 ax12_broadcast(ax12_pin, Uart(3, &uartDma), 0xfe);
	//Enable torque
	auto Ax12Tx = GpioA[2];
	auto Ax12Rx = GpioA[3];
	auto Ax12En = GpioA[1];

	Ax12 ax12_broadcast(Uart(2), 0xfe, &Ax12En, Ax12Rx, Ax12Tx);
	ax12_broadcast.setSpeed(0x3ff);
	ax12_broadcast.enable();

	while(1) {
		ax12_broadcast.goTo(0x0);
		time.msleep(2000);
		ax12_broadcast.goTo(0x280);
		time.msleep(2000);
	}
}
