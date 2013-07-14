#include <Led.h>

Led::Led(Gpio p, bool rev) :_p(p), _rev(rev) {
}

void Led::on() {
	_p.setState(!_rev);
}

void Led::off() {
	_p.setState(_rev);
}

void Led::toggle() {
	_p.setState( !_p.getState());
}
