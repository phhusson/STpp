#ifndef _INCREMENTAL_ENCODER_H
#define _INCREMENTAL_ENCODER_H
#include <Gpio.h>
#include <Timer.h>


// #TODO add support for the zero pin of the encoder
class IncrementalEncoder {
	private:
		Gpio encoderA;
		Gpio encoderB;
		Timer tim;
	public:
		IncrementalEncoder(Gpio a, Gpio B, Timer t, int lim=4096, int prescaler=0);
		int getCounter();
		operator int();
};
#endif /* _INCREMENTAL_ENCODER_H */
