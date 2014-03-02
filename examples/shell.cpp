#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Debug.h>
#include <Exti.h>
#include <UsbSerial.h>
#include <Shell.h>
#include <ShellGpio.h>

extern "C" void vTaskDelay(int);
static Shell shell;
int main() {
	UsbSerial usb;
	usb << "Hello !" << endl;
	shell.setStream(&usb, &usb);
	shell << "UserButton" << UserButton;
	shell.exec();
	while(1);
}
