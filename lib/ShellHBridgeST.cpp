#include "ShellHBridgeST.h"

Shell& operator<<(Shell& shell, HBridgeST& b) {
	if(!shell.got_name) while(1);

	addSetter(shell, b, setSpeed);

	shell.got_name = false;
	return shell;
}
