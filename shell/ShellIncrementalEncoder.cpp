#include "ShellIncrementalEncoder.h"

Shell& operator<<(Shell& shell, IncrementalEncoder& enc) {
	if(!shell.got_name) while(1);

	shell.add([&enc](Stack& s) {
		s.push(enc.getCounter());
	}, shell.current_object);

	shell.got_name = false;
	return shell;
}
