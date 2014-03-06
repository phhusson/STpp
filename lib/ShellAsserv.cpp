#include "ShellAsserv.h"

Shell& operator<<(Shell& shell, Asserv& a) {
	if(!shell.got_name) while(1);

	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.setTargetDist(v);
	}, shell.current_object, "setTargetDist");

	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.setTargetAngle(v);
	}, shell.current_object, "setTargetAngle");

	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.setProportionnalDistance(v);
	}, shell.current_object, "setProportionnalDistance");

	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.setProportionnalAngle(v);
	}, shell.current_object, "setProportionnalAngle");

	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.setIntegralDistance(v);
	}, shell.current_object, "setIntegralDistance");

	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.setIntegralAngle(v);
	}, shell.current_object, "setIntegralAngle");
	
	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.setMaxEngine(v);
	}, shell.current_object, "setMaxEngine");

	shell.add([&a,&shell](Stack& s) {
		a.reset();
	}, shell.current_object, "reset");

	shell.add([&a,&shell](Stack& s) {
		a.start();
	}, shell.current_object, "start");

	shell.add([&a,&shell](Stack& s) {
			OStream& o = *(shell.out);

			o << "Asserv" << endl;
			o << " Current Angle = " << a.getAngle() << endl;
			o << " Current Distance = " << a.getDist() << endl;
			o << " Target Angle = " << a.targetAngle << endl;
			o << " Target Distance = " << a.targetDist << endl;
			o << " Integral Angle = " << a.intAngle << endl;
			o << " Integral Distance = " << a.intDist << endl;
			o << " Left = " << a.left << endl;
			o << " Right = " << a.right << endl;
	}, shell.current_object);

	shell.got_name = false;
	return shell;
}
