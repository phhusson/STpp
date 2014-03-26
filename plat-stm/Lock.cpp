#include "Lock.h"

Mutex::Mutex() {
	vSemaphoreCreateBinary(sem);
}

void Mutex::lock() {
	xSemaphoreTake(sem, portMAX_DELAY);
}

void Mutex::unlock() {
	xSemaphoreGive(sem);
}

AutoLock::AutoLock(Mutex m)
	: _m(m) {
		_m.lock();
}

AutoLock::~AutoLock() {
	_m.unlock();
}

