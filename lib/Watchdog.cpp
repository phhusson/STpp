#include <Watchdog.h>
#include <stm32f4xx.h>

Watchdog::Watchdog(int prescaler, int autoreload) {
	int v = 0;
	prescaler >>=2;
	while(prescaler != 1) {
		prescaler >>=1;
		v++;
	}
	//IWDG->KR = 0x5555;
	IWDG->KR = 0x5555;

	autoreload &= 0xfff;
	IWDG->RLR = autoreload;

	v&=0x7;
	IWDG->PR = v;
}

Watchdog& Watchdog::start() {
	IWDG->KR = 0xCCCC;
	return *this;
}

Watchdog& Watchdog::reset() {
	IWDG->KR = 0xAAAA;
	return *this;
}

Watchdog& Watchdog::stopOnDebug() {
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
	return *this;
}

Watchdog& Watchdog::continueOnDebug() {
	DBGMCU->APB1FZ &= ~DBGMCU_APB1_FZ_DBG_IWDG_STOP;
	return *this;
}
