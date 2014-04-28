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


int main() {
	UsbSerial usb;

	Task lidar_task([&usb]() {
		Gpio LidarRX(GpioD[9]);
		Uart Lidar(3);
		LidarNeato lidar(LidarRX, Lidar);
		while(true) {
			lidar_neato_t lidar_packet;
			lidar >> lidar_packet;
		}
	}, "Lidar reader");

	usb << "Hello !" << endl;
	shell.setStream(&usb, &usb);

	Asserv asserv(Encoder1, Encoder0, Tim13, HBridge1, HBridge0);
	//Use _ prefix as a hideme for autocompletion
	shell << "_UserButton" << UserButton;

	shell << "_Prop0A" << Prop0A;
	shell << "_Prop0B" << Prop0B;

	shell << "_Prop1A" << Prop1A;
	shell << "_Prop1B" << Prop1B;

	shell << "HBridge0" << HBridge0;
	shell << "HBridge1" << HBridge1;

	shell << "Asserv" << asserv;

	shell << "Encoder0" << Encoder0;
	shell << "Encoder1" << Encoder1;

	Strategie strategie(mamoutor, asserv);
	shell << "Strategie" << strategie;

	shell << "Mamoutor" << mamoutor;

	shell.add([&asserv/*,&mamoutor*/](Stack& s) {
		(void)s;
		asserv.reset();
		//mamoutor.disable();
		Encoder0 = Encoder1 = 0;
	}, "reset");

	shell.add([&usb](Stack& s) {
		(void)s;
		int angle = s.pop().toInt();
		usb << "Lidar distance " << LidarNeato::getDistance(angle) << endl;
	}, "lidar");

	shell.add([&usb](Stack& s) {
		(void)s;
		for(int angle=0; angle<360; ++angle)
			usb << LidarNeato::getDistance(angle) << ", ";
		usb << endl;
	}, "lidar", "dump");

	shell.add([&usb,&asserv](Stack& s) {
		(void)s;
		usb << asserv.getPosition();
	}, "whereami");

	shell.exec();
	while(1);
}
