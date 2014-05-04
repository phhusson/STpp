#include <Board.h>
#include <Log.h>
#include <Ax12.h>
#include <Uart.h>

int main() {
	log << "startup" << endl;

	Zigbee_UART
		.enable()
		.setMantissa(0x16)
		.setFraction(0xb)
		.enableReceive()
		.enableTransmitter();

	Zigbee_TX
		.setPushPull()
		.setAlternate(Gpio::USART4_6)
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz);

	Zigbee_RX
		.setPushPull()
		.setAlternate(Gpio::USART4_6)
		.setDirection(Gpio::INPUT)
		.setSpeed(Gpio::SPEED_100MHz)
		.setResistor(Gpio::PULL_DOWN);
	time.msleep(500);

		//Preamble
		Zigbee_UART << (char) 0x00;
		//Start
		Zigbee_UART << (char) 0x00;
		Zigbee_UART << (char) 0xff;
		//Len
		char len = 2;

		Zigbee_UART << (char) len;
		Zigbee_UART << (char) (~len+1);

		char checksum = 0;
		//TFI
		checksum += 0xd4;
		Zigbee_UART << (char) 0xD4;
		//DATA
		checksum += 0x02;
		Zigbee_UART << (char) 0x02;

		//Data checksum
		log << "Checksum = " << (int)(~checksum+1);
		Zigbee_UART << (char)(~checksum+1);

		//Post-amble
		Zigbee_UART << (char) 0x00;

		log << "Done writing serial" << endl;
	while(1) {
		char c;
		Zigbee_UART >> c;
		log << (int)c << endl;
	}
}
