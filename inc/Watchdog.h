#ifndef _WATCHDOG_H
#define _WATCHDOG_H

class Watchdog {
	public:
		Watchdog(int prescaler, int autoreload);
		Watchdog& start();
		Watchdog& reset();
		Watchdog& stopOnDebug();
		Watchdog& continueOnDebug();
};

#endif /* _WATCHDOG_H */
