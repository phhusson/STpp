#include "ShellHBridgeST.h"

Shell& operator<<(Shell& shell, HBridgeST& b) {
	if(!shell.got_name) while(1);

	shell.add([&b,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		b.setSpeed(v);
	}, shell.current_object, "setSpeed");

	shell.got_name = false;
	return shell;
}
