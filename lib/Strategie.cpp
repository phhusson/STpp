#include "Strategie.h"
#include <Board.h>

Strategie::Strategie(Ax12& Mamoutor, Asserv& asserv) :
	mamoutor_servo(Mamoutor), asserv(asserv) {
}

void Strategie::run() {
	asserv.setTargetDist(0x30000);
	time.msleep(5000);
	mamoutor();
}

void Strategie::reset() {
	resetMamoutor();
	asserv.reset();

	time.msleep(500);
	mamoutor_servo.disable();
}

void Strategie::resetMamoutor() {
	mamoutor_servo.goTo(0x000);
}

void Strategie::mamoutor() {
	mamoutor_servo.goTo(0x280);
}
