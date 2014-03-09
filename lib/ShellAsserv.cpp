#include "ShellAsserv.h"

OStream& operator<<(OStream& o, VelocityAccel& i) {
	o << " Current Angle = " << i.getAngle() << endl;
	o << " Current Distance = " << i.getDist() << endl;
	o << " Target Angle = " << i.targetAngle << endl;
	o << " Target Distance = " << i.targetDist << endl;
	o << " Integral Angle = " << i.getIntegralAngle() << endl;
	o << " Integral Distance = " << i.getIntegralDist() << endl;
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

	addSetter(shell, a, setMaxEngine);
	addSetter(shell, a, setMinEngine);
	addSetter(shell, a, setMaxAcceleration);

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
	}, shell.current_object);

	shell.got_name = false;
	return shell;
}
