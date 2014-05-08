#include "ShellTimer.h"

Shell& operator<<(Shell& shell, Timer& tim) {
	if(!shell.got_name) while(1);

	shell.add([&tim,&shell](Stack& s) {
		s.push((int)tim);
	}, shell.current_object, "get");

	addSetter(shell, tim, setCounter);

	shell.got_name = false;
	return shell;
}
