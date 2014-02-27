#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Debug.h>
#include <Exti.h>
#include <UsbSerial.h>
#include <Shell.h>

extern "C" void vTaskDelay(int);
int main() {
	Shell shell(debug, debug);
	shell.exec();
	while(1);
}
