#include "Irq.h"

Irq::Irq(int n) : irq(n) {
}

Irq& Irq::enable() {
	return *this;
}

Irq& Irq::disable() {
	return *this;
}

Irq& Irq::setPriority(int ip) {
	return *this;
}
