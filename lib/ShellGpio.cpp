#include "ShellGpio.h"


Shell& operator<<(Shell& shell, Gpio& g) {
	if(!shell.got_name) while(1);
	shell.add([&g,&shell](Stack& s) {
		*shell.out << (g.getState() ? "# On" : "# Off" ) << endl;
	}, shell.current_object, "printState");

	shell.add([&g,&shell](Stack& s) {
		s.push( (g ? 1 : 0));
	}, shell.current_object, "state");

	shell.got_name = false;
	return shell;
}
