#include <Board.h>
#include <LedStrip.h>

int main() {
#if 0
	auto ledStripG = GpioB[5];
	//DmaStream dma(2, 3, 3);
	//auto ledStripS = Spi(1, &dma);
	//auto ledStripS = Spi(1);
	//LedStrip ledStrip(ledStripS);

	ledStripG
		.setPushPull()
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz);

	ledStripG = false;
	time.msleep(100);
	ledStripG = true;
	time.msleep(100);
	ledStripG = false;

	//ledStripS.configGpio(ledStripG);

//#if 0
	int l = 0;
	while(1) {
		for(int i=0; i<71; ++i) {
			if(i==l)
				ledStrip.push(32, 32, 32);
			else
				ledStrip.push(0, 0, 0);
		}
		ledStrip.push(0, 0, 0);

		l++;
		l %= 72;
		ledStrip.reset();
	}
#endif
	while(1);
}
