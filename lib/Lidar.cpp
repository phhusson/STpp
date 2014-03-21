#include <Gpio.h>
#include <Uart.h>
#include <Lidar.h>

LidarNeato::LidarNeato(Gpio& rx, Uart& u) {
	u
		.configGpio(rx)
		.enable()
		.setMantissa(0x16)
		.setFraction(0x9)
		.enableReceive();

	rx
		.setDirection(Gpio::INPUT);

}

IStream& operator>>(IStream& uart, lidar_neato_t& packet) {
	char c = 0;
	while(c != 0xfa) {
		uart >> c;
	}

	uart >> packet.index;

	uart >> c;
	packet.speed = c;

	uart >> c;
	packet.speed |= c << 8;

	for(int i=0;i<4;++i) {
		uart >> c;
		packet.data[i] = c;

		uart >> c;
		packet.data[i] |= c << 8;

		uart >> c;
		packet.data[i] |= c << 16;

		uart >> c;
		packet.data[i] |= c << 24;
	}

	uart >> c;
	packet.checksum = c;

	uart >> c;
	packet.checksum |= c << 8;

	return uart;
}
