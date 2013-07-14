#include <stm32f4xx.h>
#include "board.h"

extern "C" void vTaskDelay(int);
int main() {
	LedG.toggle();
	while(1) {
		LedG.toggle();
		LedO.toggle();
		//vTaskDelay(100);
		volatile int i;
		for(i=0;i<1000*1000;++i);
	}
}
