#include "ShellAx12.h"

Shell& operator<<(Shell& shell, Ax12& a) {
	if(!shell.got_name) while(1);

	addSetter(shell, a, goTo);
	addSetter(shell, a, setID);

	shell.got_name = false;
	return shell;
}
