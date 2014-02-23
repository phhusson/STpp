#ifndef _DEBUG_H
#define _DEBUG_H
#include "OStream.h"

class Debug : public OStream {
	public:
		Debug();
		virtual Debug& put(char);
		virtual Debug& endl();

		Debug& put(const char*);
		Debug& operator<<(const char*);
};

extern Debug debug;
#endif /* _LOG_H */
