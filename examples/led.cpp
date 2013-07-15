#include <Board.h>

extern "C" void vTaskDelay(int);
int main() {
	Tim4.setPrescaler(420); // 100kHz
	Tim4.setAutoReload(100); //1kHz
	Tim4.enable();

	LedG.toggle();
	int i = 1;
	while(1) {
		LedG.toggle();
		LedO.setDutyCycle(i);
		vTaskDelay(50);
		i<<=1;
		if(i==128)
			i=1;
	}
}
