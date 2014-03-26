#include "Lock.h"

Mutex::Mutex() {
	//MUST BE DONE
}

void Mutex::lock() {
}

void Mutex::unlock() {
}

AutoLock::AutoLock(Mutex m)
	: _m(m) {
		_m.lock();
}

AutoLock::~AutoLock() {
	_m.unlock();
}

