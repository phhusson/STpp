#include <Pwm.h>

Pwm::Pwm(Gpio &p, Timer &t, int n, int pre, int atr) :
	gpio(p), tim(t), chan(n) {
	int n_tim = tim.getNumber();
	if(n_tim<=2)
		gpio.setAlternate(Gpio::TIM1_2);
	else if(n_tim<=5)
		gpio.setAlternate(Gpio::TIM3_5);
	else if(n_tim<=7)
		for(;;);
	else if(n_tim<=11)
		gpio.setAlternate(Gpio::TIM8_11);
	else if(n_tim<=14)
		gpio.setAlternate(Gpio::CAN1_2_TIM12_14);
	else
		for(;;);

	gpio
		.setSpeed(Gpio::SPEED_100MHz)
		.setFunction(Gpio::ALTERNATE)
		.setDirection(Gpio::OUTPUT)
		.setResistor(Gpio::NONE)
		.setPushPull();
	tim
		.setPrescaler(pre)
		.setAutoReload(atr)
		.setOneShot(false)
		.setChannelComparator(chan, 0)
		.setChannelDirection(chan, Timer::OUTPUT)
		.setChannelMode(chan, Timer::CompareHigh)
		.setChannelOutput(chan, true)
		.enable();
}

Pwm& Pwm::setDutyCycle(float v) {
	tim.setChannelComparator(chan, v*tim.getAutoReload());
	return *this;
}

Pwm& Pwm::setDutyCycle(int percent) {
	tim.setChannelComparator(chan, percent*tim.getAutoReload()/100);
	return *this;
}

Pwm& Pwm::setComparator(unsigned int cmp) {
	tim.setChannelComparator(chan, cmp);
	return *this;
}
