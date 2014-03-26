#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include "UsbSerial.h"

UsbSerial::UsbSerial() {
	system("stty raw -echo");
}

UsbSerial::~UsbSerial() {
	system("stty -raw echo");
}

UsbSerial& UsbSerial::put(char c) {
	write(1, &c, 1);
	return *this;
}

UsbSerial& UsbSerial::endl() {
	write(1, "\r\n", 2);
	return *this;
}

int UsbSerial::get() {
	return getchar();
}

bool UsbSerial::available() {
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	select(1, &fds, NULL, NULL, &tv);
	return FD_ISSET(0, &fds);
}

void UsbSerial::wait() {
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	select(1, &fds, NULL, NULL, NULL);
}
