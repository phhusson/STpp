#include "ShellStrategie.h"

Shell& operator<<(Shell& shell, Strategie& strat) {
	if(!shell.got_name) while(1);

	shell.add([&strat](Stack& s) {
		(void) s;
		strat.run();
	}, shell.current_object, "run");

	shell.add([&strat](Stack& s) {
		(void) s;
		strat.mamoutor();
	}, shell.current_object, "mamoutor");

	shell.got_name = false;
	return shell;
}
