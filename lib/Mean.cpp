#include <Mean.h>

Mean::Mean(int v) : n(v) {
	inited = false;
}

Mean& Mean::operator+=(int v) {
	if(!inited) {
		val = v * n;
		inited = true;
	}
	val = v + (n-1) * val / n;
	return *this;
}

Mean::operator int() {
	return val/n;
}
