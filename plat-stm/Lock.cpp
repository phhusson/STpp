#include "Lock.h"

Mutex::Mutex() {
	sem = xSemaphoreCreateMutex();
}

void Mutex::lock() {
	xSemaphoreTake(sem, portMAX_DELAY);
}

void Mutex::unlock() {
	xSemaphoreGive(sem);
}

BinarySemaphore::BinarySemaphore() {
	vSemaphoreCreateBinary(sem);
}

void BinarySemaphore::take() {
	xSemaphoreTake(sem, portMAX_DELAY);
}

bool BinarySemaphore::tryTake() {
	return xSemaphoreTake(sem, portMAX_DELAY) != 0;
}

void BinarySemaphore::give() {
	xSemaphoreGive(sem);
}

AutoLock::AutoLock(Mutex m)
	: _m(m) {
		_m.lock();
}

AutoLock::~AutoLock() {
	_m.unlock();
}

