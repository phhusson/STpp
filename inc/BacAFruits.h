#ifndef _BACAFRUITS_H
#define _BACAFRUITS_H

#include <Ax12.h>

class BacAFruits {
	private:
		Ax12* external;
		Ax12* centered;
		Ax12* reservoir;
		bool left;
	public:
		BacAFruits(Ax12* external, Ax12* centered, Ax12* reservoir, bool left);
		BacAFruits& forward();
		BacAFruits& reject(bool leftFruit, bool rightFruit);
		BacAFruits& close();
		BacAFruits& release();
};

#endif /* _BACAFRUITS_H */
