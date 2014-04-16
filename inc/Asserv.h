#ifndef _ASSERV_H
#define _ASSERV_H
#include <HBridgeST.h>
#include <IncrementalEncoder.h>
#include <OStream.h>
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
		friend OStream& operator<<(OStream&, VelocityAccel&);
		friend class Asserv;
	public:
		int maxAccel, minAccel;
		int maxVel,minVel;
};

class Position {
	private:
		VelocityAccel& infos;
		float x;
		float y;
		float theta;
		int previousEDist;
		float wheelSize, wheelDist;
	public:
		Position(VelocityAccel& infos, float wheelSize, float wheelDist);
		void update();
		float getX();
		float getY();
		float getTheta();
		friend OStream& operator<<(OStream&, Position&);
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
		Position position;

		int targetAngle, targetDist;
		int c_propDist, c_propAngle;
		int c_intDist, c_intAngle;
		int c_velDist, c_velAngle;
		int c_accelDist, c_accelAngle;

		int maxEngine, minEngine;
		int maxForwardAccel, maxBackwardAccel;

		bool waiting;
		int beenZero;

		int throttle;
		int date;
		int dateStart;

	public:
		Asserv(IncrementalEncoder& left, IncrementalEncoder& right, Timer& tim, HBridgeST& mot1, HBridgeST& mot2);
		Asserv& setTargetDist(int t);
		Asserv& setTargetAngle(int a);

		Asserv& angle(int a);
		Asserv& dist(int t);
		Position& getPosition();

		Asserv& setMaxAcceleration(int l);
		Asserv& setMaxEngine(int l);
		Asserv& setMinEngine(int l);

		Asserv& setProportionnalDist(int c);
		Asserv& setProportionnalAngle(int c);
		Asserv& setIntegralDist(int c);
		Asserv& setIntegralAngle(int c);
		Asserv& setVelocityDist(int c);
		Asserv& setVelocityAngle(int c);
		Asserv& setAccelDist(int c);
		Asserv& setAccelAngle(int c);

		Asserv& reset();
		Asserv& start();

		int getAngle();
		int getDist();
		friend Shell& operator<<(Shell&, Asserv&);
};
#endif /* _ASSERV_H */
