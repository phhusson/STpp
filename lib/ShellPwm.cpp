#include "ShellPwm.h"

Shell& operator<<(Shell& shell, Pwm& pwm) {
	if(!shell.got_name) while(1);

	addSetter(shell, pwm, setComparator);
	addSetter(shell, pwm, setDutyCycle);

	shell.got_name = false;
	return shell;
}
