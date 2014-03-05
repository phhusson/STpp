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
		HBridgeST& motorl;
		HBridgeST& motorr;
		int angl, dist;
		int kr, kl, rr, rl;
		int max_engine;

	public:
		Asserv(IncrementalEncoder& left, IncrementalEncoder& right, Timer& tim, HBridgeST mot1, HBridgeST mot2);
		Asserv& setTargetDist(int t);
		Asserv& setTargetAngle(int a);
		Asserv& setPCorrectors(int l, int r);
		Asserv& setMaxEngine(int l);

		friend Shell& operator<<(Shell&, Asserv&);
};
#endif /* _ASSERV_H */
