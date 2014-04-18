#include <Board.h>
#include <LedStrip.h>

int main() {
	auto ledStripG = GpioB[5];
	DmaStream dma(2, 3, 3);
	auto ledStripS = Spi(1, &dma);
	//auto ledStripS = Spi(1);
	LedStrip ledStrip(ledStripS);

	ledStripG
		.setPushPull()
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz);

	ledStripG = false;
	time.msleep(2);
	ledStripG = true;
	time.msleep(2);
	ledStripG = false;
	time.msleep(2);
	ledStripG = true;
	ledStripS.configGpio(ledStripG);
	time.msleep(4);

	int l = 0;
	while(1) {
		for(int i=0; i<71; ++i) {
			if(l==i)
				ledStrip.push(0, 255, 0);
			else
				ledStrip.push(6, 0, 0);
		}
		ledStrip.push(0, 0, 0);

		l++;
		l %= 72;
		ledStrip.reset();
		//time.msleep(200);
	}
}
