#include "Strategie.h"
#include <Board.h>

Strategie::Strategie(Ax12& Mamoutor, Asserv& asserv) :
	mamoutor_servo(Mamoutor), asserv(asserv) {
}

void Strategie::run() {
	asserv.dist(10000);
	time.msleep(2000);
	//asserv.wait();
	asserv.angle(-2500);
	time.msleep(2000);
	//asserv.wait();
	asserv.dist(8000);
	time.msleep(2000);
	//asserv.wait();
	mamoutor();
	time.msleep(2000);
	resetMamoutor();
}

void Strategie::reset() {
	resetMamoutor();
	asserv.reset();

	time.msleep(500);
	mamoutor_servo.disable();
}

void Strategie::resetMamoutor() {
	mamoutor_servo.goTo(0x200);
}

void Strategie::mamoutor() {
	mamoutor_servo.goTo(0x300);
}
