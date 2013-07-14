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

class AutoLock {
	private:
		Mutex _m;
	public:
		AutoLock(Mutex m);
		~AutoLock();
};
