#include "Debug.h"

extern "C" void putchar(char);
Debug& Debug::put(char c) {
	putchar(c);
	return *this;
}

extern "C" void putstring(const char*);
Debug& Debug::put(const char *s) {
	putstring(s);
	return *this;
}

Debug& Debug::endl() {
	put('\n');
	return *this;
}

Debug::Debug() {
}

Debug& Debug::operator<<(const char* s) {
	putstring(s);
	return *this;
}

extern "C" char getchar();
int Debug::get() {
	return getchar();
}

bool Debug::available() {
	return true;
}

Debug debug;
