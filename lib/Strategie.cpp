#include "Strategie.h"

extern "C" void vTaskDelay(int time);
Strategie::Strategie(Ax12& Mamoutor, Asserv& asserv) :
	mamoutor_servo(Mamoutor), asserv(asserv) {
}

void Strategie::run() {
	asserv.setTargetDist(0x30000);
	vTaskDelay(5000);
	mamoutor();
}

void Strategie::reset() {
	resetMamoutor();
	asserv.reset();

	vTaskDelay(500);
	mamoutor_servo.disable();
}

void Strategie::resetMamoutor() {
	mamoutor_servo.goTo(0x200);
}

void Strategie::mamoutor() {
	mamoutor_servo.goTo(0x200);
}
