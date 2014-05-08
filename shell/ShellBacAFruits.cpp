#include "ShellBacAFruits.h"

Shell& operator<<(Shell& shell, BacAFruits& bac) {
	if(!shell.got_name) while(1);

	shell.add([&bac,&shell](Stack& s) {
		(void) s;
		bac.forward();
	}, shell.current_object, "forward");

	shell.add([&bac,&shell](Stack& s) {
		(void) s;
		bac.close();
	}, shell.current_object, "close");

	shell.got_name = false;
	return shell;
}
