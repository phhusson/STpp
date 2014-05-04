#include "Bluetooth.h"

Bluetooth::Bluetooth(int nb, Gpio& tx, Gpio& rx): Uart(nb) {
	//setBaudrate(9600);
	enable();
	enableTransmitter();
	enableReceive();
	
	configGpio(tx);
	configGpio(rx);

	tx
		.setDirection(Gpio::OUTPUT);

	rx
		.setDirection(Gpio::INPUT)
		.setResistor(Gpio::PULL_DOWN);


}
