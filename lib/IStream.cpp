#include <IStream.h>

IStream& operator>>(IStream& i, char &c) {
	while(!i.available());
	c = i.get();
	return i;
}

IStream& operator>>(IStream& i, int &c) {
	//TODO
	while(1);
	return i;
}

IStream& operator>>(IStream& s, char* str) {
	for(int i=0;; ++i) {
		while(!s.available());
		str[i] = s.get();
		if(str[i] == '\n' || str[i] == '\r' || str[i] == ' ')
			str[i]=0;
		if(str[i] == 0) {
			if(i==0)
				--i;
			else
				break;
		}
	}
	return s;
}
