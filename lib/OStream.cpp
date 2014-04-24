#include <OStream.h>

Endl endl;

OStream& OStream::put(char *s, int l) {
	for(int i=0; i<l; ++i) {
		put(s[i]);
	}
	return *this;
}

OStream& operator<<(OStream& o, char c) {
	o.put(c);
	return o;
}

OStream& operator<<(OStream& o, const char *str) {
	while(*str)
		o.put(*str++);
	return o;
}

OStream& operator<<(OStream& o, int i) {
	int pos = 7;
	o << "0x";
	while(pos>=0) {
		char c = (i >> (pos*4)) & 0xf;
		if(c>9)
			c+='a' - 10;
		else
			c+='0';
		o << c;

		pos--;
	}
	return o;
}

OStream& operator<<(OStream& o, Endl e) {
	o.endl();
	return o;
}
