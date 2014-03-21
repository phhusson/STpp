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
#include <Lidar.h>

int main() {
	UsbSerial usb;
	usb << "Hello !" << endl;

	Gpio LidarRX(GpioA[3]);
	Uart Lidar(2);
	LidarNeato lidar(LidarRX, Lidar);

	while(true) {
		lidar_neato_t packet;
		Lidar >> packet;

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
