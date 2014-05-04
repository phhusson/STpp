#ifndef LOCK_H
#define LOCK_H

extern "C" {
#include <FreeRTOS.h>
#include <semphr.h>
};

class Mutex {
	private:
		xSemaphoreHandle sem;
	public:
		Mutex();
		void lock();
		void unlock();
};

class BinarySemaphore {
	private:
		xSemaphoreHandle sem;
	public:
		BinarySemaphore();
		void take();
		void give();
		bool tryTake();
};

class AutoLock {
	private:
		Mutex _m;
	public:
		AutoLock(Mutex m);
		~AutoLock();
};

#endif /* LOCK_H */
