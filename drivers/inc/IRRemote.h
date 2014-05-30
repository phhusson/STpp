#ifndef _IRREMOTE_H
#define _IRREMOTE_H

#include <Timer.h>
#include <Gpio.h>
#include <Lock.h>

class IRRemote {
	private:
		Timer& t;
		Gpio& g;

		BinarySemaphore dataAvailable;
		BinarySemaphore dataReleased;
		uint32_t word;
		uint32_t tmpWord;
	public:
		IRRemote(Timer& t, Gpio& g);
		int nextRaw();
		int next();
};

#endif /* _IRREMOTE_H */
