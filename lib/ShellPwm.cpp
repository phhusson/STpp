#include "ShellPwm.h"

Shell& operator<<(Shell& shell, Pwm& pwm) {
	if(!shell.got_name) while(1);

	shell.add([&pwm,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		pwm.setComparator(v);
	}, shell.current_object, "setComparator");

	shell.add([&pwm,&shell](Stack& s) {
		Object& o = s.pop();
		if(o.type != Object::INT) while(1);
		int v = o.toInt();

		pwm.setDutyCycle(v);
	}, shell.current_object, "setDutyCycle");

	shell.got_name = false;
	return shell;
}
