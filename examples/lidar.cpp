#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Debug.h>
#include <Exti.h>
#include <UsbSerial.h>
#include <Shell.h>
#include <ShellAsserv.h>
#include <ShellAx12.h>
#include <ShellGpio.h>
#include <ShellHBridgeST.h>
#include <ShellIncrementalEncoder.h>
#include <ShellPwm.h>
#include <ShellStrategie.h>
#include <ShellTimer.h>

int main() {
	UsbSerial usb;
	usb << "Hello !" << endl;

	Gpio LidarTX(GpioA[2]);
	Gpio LidarRX(GpioA[3]);
	Uart Lidar(2);
	Lidar
		.configGpio(LidarTX)
		.configGpio(LidarRX)
		.enable()
		.setMantissa(0x16)
		.setFraction(0x9)
		.enableReceive();

	LidarRX
		.setDirection(Gpio::INPUT);
	while(true) {
		char c = 0;
		while(c != 0xfa) {
			Lidar >> c;
		}

		struct {
			char index;
			uint16_t speed;
			uint32_t data[4];
			uint16_t checksum;
		} packet;
		Lidar >> packet.index;

		Lidar >> c;
		packet.speed = c;

		Lidar >> c;
		packet.speed |= c << 8;

		for(int i=0;i<4;++i) {
			Lidar >> c;
			packet.data[i] = c;

			Lidar >> c;
			packet.data[i] |= c << 8;

			Lidar >> c;
			packet.data[i] |= c << 16;

			Lidar >> c;
			packet.data[i] |= c << 24;
		}

		Lidar >> c;
		packet.checksum = c;

		Lidar >> c;
		packet.checksum |= c << 8;

		if(packet.index == 0xa9) {
		usb << "["
				  /*<< (int)packet.index
				  << "]"*/
				  << "@"
				  << (int)packet.speed
				  /*
				  << ":"
				  << (int)packet.data[0]
				  << "," << (int)packet.data[1]
				  << "," << (int)packet.data[2]
				  << "," << (int)packet.data[3]*/
				  << "=" << (int)(packet.data[0]&0x7fff)
				  << ";" << endl;
		}
	}

	while(1);
}
