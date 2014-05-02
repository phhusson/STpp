#include <BacAFruits.h>
#include <Board.h>
#include <Log.h>

BacAFruits::BacAFruits(Ax12* external,
		Ax12* centered,
		Ax12* reservoir,
		bool left) :
	external(external), centered(centered),
	reservoir(reservoir), left(left) {
}

BacAFruits& BacAFruits::forward() {
	int pos = 0x200 + left ? 0x80  : -0x80;
	reservoir->goTo(pos);

	if(external) {
		//Two arms method
		int ePos = 0x200 + left ? 0x40 : -0x40;
		int cPos = 0x200 + !left ? 0x40 : -0x40;

		centered->goTo(cPos);
		external->goTo(ePos);
	} else {
		//One arm method
		centered->goTo(0x200);
	}

	return *this;
}

BacAFruits& BacAFruits::close() {
	int posFullyClosed = 0x200 + left ? 0x130  : -0x130;
	reservoir->goTo(posFullyClosed);

	time.msleep(1000);
	int posHalfClosed = 0x200 + left ? 0x100  : -0x100;
	reservoir->goTo(posHalfClosed);
	time.msleep(300);

	int ePos = 0x200 + left ? 0x150 : -0x150;
	int cPos = 0x200 + left ? 0x170 : -0x170;
	external->goTo(ePos);
	centered->goTo(cPos);

	return *this;
}
