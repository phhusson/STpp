#include "board.h"

extern "C" void vTaskDelay(int);
int main() {
	while(1) {
		LedR.toggle();
		vTaskDelay(100);
	}
}
