#include "board.h"

extern void sleep(int);
int main() {
	while(1) {
		LedR.toggle();
		sleep(1);
	}
}
