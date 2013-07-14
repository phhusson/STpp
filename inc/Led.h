#include "Gpio.h"
#include "Lock.h"

class Led {
	private:
		Mutex _m;
		Gpio _p;
		bool _rev;
	public:
		Led(Gpio p, bool rev=false);
		void on();
		void off();
		void toggle();
};
