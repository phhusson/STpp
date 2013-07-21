#include "Log.h"

Log& Log::operator<<(char c) {
	log[line][position]=c;
	position++;
	return *this;
}

Log& Log::operator<<(const char* str) {
	int i;
	for(i=0;str[i] && (i+position) < sizeof(log[0]);++i) {
		log[line][i+position]=str[i];
	}
	position+=i;
	return *this;
}

Log& Log::operator<<(int i) {
	int pos = 7;
	*this << "0x";
	while(pos>=0) {
		char c = (i >> (pos*4)) & 0xf;
		if(c>9)
			c+='a' - 10;
		else
			c+='0';
		*this << c;

		pos--;
	}
	return *this;
}

Log& Log::operator<<(Log::endl_type e) {
	log[line][position]=0;
	line++;
	line%=sizeof(log)/sizeof(log[0]);
	position = 0;
	return *this;
}
