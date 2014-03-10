#include "ShellAsserv.h"

OStream& operator<<(OStream& o, VelocityAccel& i) {
	o << " Current Angle = " << i.getAngle() << endl;
	o << " Current Distance = " << i.getDist() << endl;
	o << " Target Angle = " << i.targetAngle << endl;
	o << " Target Distance = " << i.targetDist << endl;
	o << " Delta Angle = " << i.getDeltaAngle() << endl;
	o << " Delta Distance = " << i.getDeltaDist() << endl;
	o << " Integral Angle = " << i.getIntegralAngle() << endl;
	o << " Integral Distance = " << i.getIntegralDist() << endl;
	o << endl;

	o << " Max Accel = " << i.maxAccel << endl;
	o << " Min Accel = " << i.minAccel << endl;

	o << " Max Vel = " << i.maxVel << endl;
	o << " Min Vel = " << i.minVel << endl;
	return o;
}

Shell& operator<<(Shell& shell, Asserv& a) {
	if(!shell.got_name) while(1);

	addSetter(shell, a, setTargetAngle);
	addSetter(shell, a, setTargetDist);

	addSetter(shell, a, setVelocityAngle);
	addSetter(shell, a, setVelocityDist);
	addSetter(shell, a, setProportionnalAngle);
	addSetter(shell, a, setProportionnalDist);
	addSetter(shell, a, setIntegralAngle);
	addSetter(shell, a, setIntegralDist);
	addSetter(shell, a, setAccelAngle);
	addSetter(shell, a, setAccelDist);

	addSetter(shell, a, setMaxEngine);
	addSetter(shell, a, setMinEngine);
	addSetter(shell, a, setMaxAcceleration);

	addSetter(shell, a, angle);
	addSetter(shell, a, dist);

	shell.add([&a,&shell](Stack& s) {
		a.reset();
	}, shell.current_object, "reset");

	shell.add([&a,&shell](Stack& s) {
		a.start();
	}, shell.current_object, "start");

	shell.add([&a,&shell](Stack& s) {
		OStream& o = *(shell.out);

		o << "Asserv" << endl;
		o << a.infos;
		o << "Coefficients" << endl;
		o << "Dist.... Angle..." << endl;

		o << "     Proportional" << endl;
		o << a.c_propDist << " " << a.c_propAngle << endl;

		o << "     Integral" << endl;
		o << a.c_intDist << " " << a.c_intAngle << endl;

		o << "     Derivative" << endl;
		o << a.c_velDist << " " << a.c_velAngle << endl;

		o << "     Acceleration" << endl;
		o << a.c_accelDist << " " << a.c_accelAngle << endl;
	}, shell.current_object);

	shell.got_name = false;
	return shell;
}
