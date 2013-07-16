#include <Led.h>

Led::Led(Gpio p, bool rev) :_p(p), _rev(rev) {
	p.setDirection(Gpio::OUTPUT);
	p.setPushPull();
	p.setSpeed(Gpio::SPEED_50MHz);
	p.setFunction(Gpio::GPIO);
	on();
}

Led& Led::on() {
	_p.setState(!_rev);
	return *this;
}

Led& Led::off() {
	_p.setState(_rev);
	return *this;
}

Led& Led::toggle() {
	_p.setState( !_p.getState());
	return *this;
}
