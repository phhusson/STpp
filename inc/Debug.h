#ifndef _DEBUG_H
#define _DEBUG_H
#include "OStream.h"
#include "IStream.h"

class Debug : public OStream, public IStream {
	public:
		Debug();
		virtual Debug& put(char);
		virtual Debug& endl();

	public:
		Debug& put(const char*);
		Debug& operator<<(const char*);

	public:
		bool available();
		int get();
};

extern Debug debug;
#endif /* _LOG_H */
