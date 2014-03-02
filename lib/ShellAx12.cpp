#include "ShellAx12.h"

Shell& operator<<(Shell& shell, Ax12& a) {
	if(!shell.got_name) while(1);

	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.goTo(v);
	}, shell.current_object, "goTo");

	shell.add([&a,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		a.setID(v);
	}, shell.current_object, "setId");

	shell.got_name = false;
	return shell;
}
