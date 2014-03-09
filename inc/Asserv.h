#ifndef _ASSERV_H
#define _ASSERV_H
#include <HBridgeST.h>
#include <IncrementalEncoder.h>
#include <Timer.h>

class Shell;

class VelocityAccelPerEncoderCompute {
	private:
		int last;
		int lastVelocity;
		int lastAccel;

	public:
		VelocityAccelPerEncoderCompute();
		void compute(int value);
		int getVelocity();
		int getAccel();
		int getCurrent();
		operator int();
		void operator()(int);
		int operator=(int);
};

class VelocityAccel {
	private:
		VelocityAccelPerEncoderCompute &left, right;
		IncrementalEncoder& eLeft, eRight;
		int intDist, intAngle;
		int targetDist, targetAngle;

		int N, D;
	public:
		VelocityAccel(VelocityAccelPerEncoderCompute& left, VelocityAccelPerEncoderCompute &right, IncrementalEncoder& eLeft, IncrementalEncoder& eRight, int N_LR, int D_LR);
		void compute(int dist, int angle);

		int getIntegralAngle();
		int getIntegralDist();

		int getAngle();
		int getDeltaAngle();
		int getDist();
		int getDeltaDist();

		int getVelocityAngle();
		int getVelocityDist();

		int getAccelAngle();
		int getAccelDist();
};

class Asserv {
	private:
		Timer& tim;
		IncrementalEncoder& eLeft;
		IncrementalEncoder& eRight;
		HBridgeST& motorl;
		HBridgeST& motorr;

		VelocityAccelPerEncoderCompute left;
		VelocityAccelPerEncoderCompute right;

		VelocityAccel infos;

		int targetAngle, targetDist;
		int c_propDist, c_propAngle;
		int c_intDist, c_intAngle;
		int c_velDist, c_velAngle;

		int maxEngine, minEngine;
		int maxAccel;

		bool waiting;

	public:
		Asserv(IncrementalEncoder& left, IncrementalEncoder& right, Timer& tim, HBridgeST mot1, HBridgeST mot2);
		Asserv& setTargetDist(int t);
		Asserv& setTargetAngle(int a);

		Asserv& setMaxAcceleration(int l);
		Asserv& setMaxEngine(int l);
		Asserv& setMinEngine(int l);

		Asserv& setProportionnalDist(int c);
		Asserv& setProportionnalAngle(int c);
		Asserv& setIntegralDist(int c);
		Asserv& setIntegralAngle(int c);
		Asserv& setVelocityDist(int c);
		Asserv& setVelocityAngle(int c);

		Asserv& reset();
		Asserv& start();

		int getAngle();
		int getDist();
		friend Shell& operator<<(Shell&, Asserv&);
};
#endif /* _ASSERV_H */
