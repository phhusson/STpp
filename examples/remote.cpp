#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Debug.h>
#include <Exti.h>
#include <UsbSerial.h>
#include <Shell.h>
#include <ShellAsserv.h>
#include <ShellAx12.h>
#include <ShellBacAFruits.h>
#include <ShellGpio.h>
#include <ShellHBridgeST.h>
#include <ShellIncrementalEncoder.h>
#include <ShellPwm.h>
#include <ShellStrategie.h>
#include <ShellTimer.h>
#include <Lidar.h>
#include <Lidar.h>
#include <Tasks.h>
#include <RPC.h>

static RPC rpc;

int main() {
	UsbSerial usb;

	rpc.setStream(&usb, &usb);
	rpc.registerClass(0x00, [&usb](int len, char *msg) {
		for(int i=0; i<len; ++i) {
			usb << (char)msg[i];
		}
	});
	rpc.run();

	return 0;
}
