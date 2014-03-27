#include <Gpio.h>
#include <Uart.h>
#include <Lidar.h>

LidarNeato::LidarNeato(Gpio& rx, Uart& u): uart(u) {
	u
		.configGpio(rx)
		.enable()
		.setMantissa(0x16)
		.setFraction(0x9)
		.enableReceive();

	rx
		.setDirection(Gpio::INPUT);

}

LidarNeato& operator>>(LidarNeato& lidar, lidar_neato_t& packet) {
	Uart& uart = lidar.uart;
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
	{
		AutoLock a(lidar.distancesLock);
		for(int i=0; i<4; ++i) {
			int pos = (packet.index-0xa0)*4 + i;
			//Should be reported
			if(pos < 0 || pos >= 360)
				continue;
			lidar.distances[ pos ] = packet.data[i] & 0x3fff;
		}
	}

	return lidar;
}
uint16_t LidarNeato::getDistance(int angle) {
	AutoLock a(distancesLock);
	return distances[angle];
}
