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

		a.setPCorrector(v);
	}, shell.current_object, "setPCorrector");

	shell.add([&a,&shell](Stack& s) {
			OStream& o = *(shell.out);

			o << "Asserv" << endl;
			o << " ki = " << a.ki << endl;
			o << " Target Angle = " << a.angl << endl;
			o << " Target Distance = " << a.dist << endl;
			o << " Left = " << a.left << endl;
			o << " Right = " << a.right << endl;
	}, shell.current_object);

	shell.got_name = false;
	return shell;
}
