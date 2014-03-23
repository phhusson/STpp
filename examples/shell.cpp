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
#include <Lidar.h>
#include <Tasks.h>

static Shell shell;

static lidar_neato_t lidar_packet;

int main() {
	UsbSerial usb;

	Mutex lidar_lock;
	Task lidar_task([]() {
		Gpio LidarRX(GpioD[9]);
		Uart Lidar(3);
		LidarNeato lidar(LidarRX, Lidar);
		while(true) {
			Lidar >> lidar_packet;
		}
	}, "Lidar reader");

	usb << "Hello !" << endl;
	shell.setStream(&usb, &usb);

	//Use _ prefix as a hideme for autocompletion
	shell << "_UserButton" << UserButton;

	shell << "_Prop0A" << Prop0A;
	shell << "_Prop0B" << Prop0B;

	shell << "_Prop1A" << Prop1A;
	shell << "_Prop1B" << Prop1B;

	shell << "HBridge0" << HBridge0;
	shell << "HBridge1" << HBridge1;

	Asserv asserv(Encoder1, Encoder0, Tim13, HBridge1, HBridge0);

	shell << "Asserv" << asserv;

	shell << "Encoder0" << Encoder0;
	shell << "Encoder1" << Encoder1;

	//Strategie strategie(mamoutor, asserv);
	//shell << "Strategie" << strategie;

	shell.add([&asserv/*,&mamoutor*/](Stack& s) {
		asserv.reset();
		//mamoutor.disable();
		Encoder0 = Encoder1 = 0;
	}, "reset");

	shell.add([&usb, &lidar_lock](Stack& s) {
		AutoLock l(lidar_lock);
		usb << "@" << (int) lidar_packet.speed << endl;
		for(int i=0; i<4; ++i) {
			usb
				<< "#"
				<< ((int)(lidar_packet.index-0xa0))*4+i
				<< ","
				<< ((int)lidar_packet.data[i]&0x3fff)
				<< ((lidar_packet.data[i]&0x4000) ? "!" : "")
				<< endl;
		}
	}, "lidar");

	shell.add([&usb, &asserv](Stack& s) {
		usb << asserv.getPosition();
	}, "whereami");

	shell.exec();
	while(1);
}
