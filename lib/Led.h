#include "Gpio.h"
#include "Lock.h"

class Led {
	private:
		Mutex m;
	public:
		Led(Gpio);
		void on();
		void off();
		void toggle();
};
