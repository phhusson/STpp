#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Timer.h>
#include <Irq.h>
#include <Spi.h>

int main() {
	auto ledStripeG = GpioB[5];
	auto ledStripe = Spi(1);

	ledStripeG
		.setPushPull()
		.setDirection(Gpio::OUTPUT)
		.setAlternate(Gpio::SPI1_2)
		.setSpeed(Gpio::SPEED_100MHz);

	ledStripe
		.enable()
		.setDivisorPow2(4)
		.master();

	while(1) {
		//110 110 11.
		ledStripe.send(0b11011011);
		//..0 110 110 1..
		ledStripe.send(0b01101101);
		//.10 110 110
		ledStripe.send(0b10110110);

		for(int i=0; i<500; ++i)
			ledStripe.send(0);
	}
}
