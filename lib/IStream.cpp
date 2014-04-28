#include <IStream.h>

IStream& operator>>(IStream& i, char &c) {
	i.wait();
	c = i.get();
	return i;
}

IStream& operator>>(IStream& i, int &c) {
	(void)c;
	//TODO
	while(1);
	return i;
}

IStream& operator>>(IStream& s, char* str) {
	for(int i=0;; ++i) {
		s.wait();
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

void IStream::wait() {
	while(!available());
}
