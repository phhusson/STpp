#ifndef _ASSERV_H
#define _ASSERV_H
#include <IncrementalEncoder.h>
#include <Timer.h>

class Asserv {
	private:
		Timer& tim;
		IncrementalEncoder& left;
		IncrementalEncoder& right;

	public:
		Asserv(IncrementalEncoder& left, IncrementalEncoder& right,
			Timer& tim);
};
#endif /* _ASSERV_H */
