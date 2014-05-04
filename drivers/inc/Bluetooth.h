#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H
#include <Uart.h>

class Bluetooth: public Uart {
	private:
		const char *getAnswer();
	public:
		Bluetooth(int nb, Gpio& tx, Gpio& rx);
		Bluetooth& setName(const char*);
};

#endif /* _BLUETOOTH_H */
