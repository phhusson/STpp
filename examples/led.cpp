#include <stm32f4xx.h>
#include <Board.h>

extern "C" void vTaskDelay(int);
int main() {
	LedG.toggle();
	while(1) {
		LedG.toggle();
		vTaskDelay(500);
	}
}
