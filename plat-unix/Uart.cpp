#include <unistd.h>
#include <Uart.h>
#include <Log.h>
#include <Irq.h>
#include <stm32f4xx.h>

Uart::Uart(int n) {
	number = 0;
}

Uart& Uart::configGpio(Gpio& p) {
	return *this;
}

Uart& Uart::put(char c) {
	return *this;
}

Uart& Uart::setMantissa(int m) {
	return *this;
}

Uart& Uart::setFraction(int f) {
	return *this;
}

Uart& Uart::enableReceive() {
	return *this;
}

Uart& Uart::disableReceive() {
	return *this;
}

Uart& Uart::enableTransmitter() {
	return *this;
}

Uart& Uart::disableTransmitter() {
	return *this;
}

Uart& Uart::enable() {
	return *this;
}

Uart& Uart::disable() {
	return *this;
}

char Uart::waitForNext() {
	return 0;
}

Uart& Uart::sendBreak() {
	return *this;
}

Uart& Uart::setHalfDuplex(bool b) {
	return *this;
}

Uart& Uart::endl() {
	return *this;
}

int Uart::get() {
	return -1;
}

bool Uart::available() {
	return false;
}

void Uart::wait() {
	while(1) usleep(1000*100);
}
