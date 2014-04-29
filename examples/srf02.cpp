#include <Board.h>
#include <Log.h>
#include <Ax12.h>
#include <Uart.h>
#include <Debug.h>

int main() {
	log << "startup" << endl;

	//115200 = 38400 * 3 = 9600 * 12
	//
	//38400
	//0x43b
	//
	//9600
	//0x10ec
	ExternalUart
		.enable()
		.setMantissa(0x10e)
		.setFraction(0xc)
		.enableReceive()
		.enableTransmitter()
		.configGpio(External_RX, External_TX);


	External_TX.setDirection(Gpio::OUTPUT);
	External_RX
		.setResistor(Gpio::PULL_UP);

	char addr = 1;
	while(1) {
		static char pulse[] __attribute__((section("dma"))) = "\x01\x51";
		pulse[0]=addr;
		ExternalUart.put(pulse, sizeof(pulse));
		time.msleep(300);

		static char result[] __attribute__((section("dma"))) = "\x01\x5e";
		result[0]=addr;
		ExternalUart.put(result, sizeof(result));
		char c1,c2;
		ExternalUart >> c1 >> c2;
		debug << (int)(c1 << 8 | c2) << endl;

		time.msleep(500);
	}
}
