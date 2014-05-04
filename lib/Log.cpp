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

void Log::dump(OStream& o) {
	for(unsigned i=0; i<sizeof(log)/sizeof(*log); ++i) {
		unsigned p = (position + i+1) % sizeof(log)/sizeof(*log);
		o << (char*)log[p] << ::endl;
	}
}
