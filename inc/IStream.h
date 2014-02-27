#ifndef _ISTREAM_H
#define _ISTREAM_H

class IStream {
	public:
		virtual int get() = 0;
		virtual bool available() = 0;
};

IStream& operator>>(IStream& i, char &c);
IStream& operator>>(IStream& i, int &c);
IStream& operator>>(IStream& i, char* str);
//Doesn't work...
template<int n> IStream& operator>>(IStream& s, char str[n]) {
	for(int i=0; i<n; ++i) {
		while(!s.available());
		str[i] = s.get();
		if(str[i] == '\n' || str[i] == '\r')
			str[i]=0;
		if(str[i] == 0) {
			if(i==0)
				--i;
			else
				break;
		}
	}
	return s;
};

#endif /* _ISTREAM_H */
