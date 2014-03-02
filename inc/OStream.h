#ifndef _OSTREAM_H
#define _OSTREAM_H

class Endl {};
extern Endl endl;
class OStream {
	public:
		virtual OStream& put(char) = 0;
		virtual OStream& endl() = 0;
};

class NullOStream : public OStream {
	public:
		virtual OStream& put(char c) { return *this; }
		virtual OStream& endl() { return *this; }
};

OStream& operator<<(OStream& o, char c);
OStream& operator<<(OStream& o, const char* str);
OStream& operator<<(OStream& o, int i);
OStream& operator<<(OStream& o, Endl e);
#endif /* _OSTREAM_H */
