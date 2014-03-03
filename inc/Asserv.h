#ifndef _ASSERV_H
#define _ASSERV_H
#include <HBridgeST.h>
#include <IncrementalEncoder.h>
#include <Timer.h>

class Shell;
class Asserv {
	private:
		Timer& tim;
		IncrementalEncoder& left;
		IncrementalEncoder& right;
		HBridgeST& motor1;
		HBridgeST& motor2;
		int angl, dist;
		int ki;

	public:
		Asserv(IncrementalEncoder& left, IncrementalEncoder& right, Timer& tim, HBridgeST mot1, HBridgeST mot2);
		Asserv& setTargetDist(int t);
		Asserv& setTargetAngle(int a);
		Asserv& setPCorrector(int p);
		

		friend Shell& operator<<(Shell&, Asserv&);
};
#endif /* _ASSERV_H */
