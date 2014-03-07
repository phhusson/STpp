#ifndef _ASSERV_H
#define _ASSERV_H
#include <HBridgeST.h>
#include <IncrementalEncoder.h>
#include <Timer.h>

class Shell;

class VelocityAccelCompute {
	private:
		int lastInt;
		int last;
		int lastVelocity;
		int lastAccel;

	public:
		VelocityAccelCompute();
		void compute(int value);
		int getIntegral();
		int getVelocity();
		int getAccel();
		int getCurrent();
		operator int();
		void operator()(int);
		int operator=(int);
};

class Asserv {
	private:
		Timer& tim;
		IncrementalEncoder& eLeft;
		IncrementalEncoder& eRight;
		HBridgeST& motorl;
		HBridgeST& motorr;

		VelocityAccelCompute left;
		VelocityAccelCompute right;

		int targetAngle, targetDist;
		int c_propDist, c_propAngle, c_intDist, c_intAngle;
		int intDist, intAngle;

		int maxEngine, minEngine;

	public:
		Asserv(IncrementalEncoder& left, IncrementalEncoder& right, Timer& tim, HBridgeST mot1, HBridgeST mot2);
		Asserv& setTargetDist(int t);
		Asserv& setTargetAngle(int a);

		Asserv& setMaxEngine(int l);
		Asserv& setMinEngine(int l);

		Asserv& setProportionnalDistance(int c);
		Asserv& setProportionnalAngle(int c);
		Asserv& setIntegralDistance(int c);
		Asserv& setIntegralAngle(int c);

		Asserv& reset();
		Asserv& start();

		int getAngle();
		int getDist();
		friend Shell& operator<<(Shell&, Asserv&);
};
#endif /* _ASSERV_H */
