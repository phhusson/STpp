#include "Log.h"

Log& Log::put(char c) {
	log[line][position]=c;
	position++;
	return *this;
}

Log& Log::endl() {
	log[line][position]=0;
	line++;
	line%=sizeof(log)/sizeof(log[0]);
	position = 0;
	return *this;
}
