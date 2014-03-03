#include "ShellTimer.h"

Shell& operator<<(Shell& shell, Timer& tim) {
	if(!shell.got_name) while(1);

	shell.add([&tim,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		tim.setCounter(v);
	}, shell.current_object, "set");

	shell.got_name = false;
	return shell;
}
