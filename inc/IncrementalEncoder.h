#ifndef _INCREMENTAL_ENCODER_H
#define _INCREMENTAL_ENCODER_H
#include <Gpio.h>
#include <Timer.h>


// #TODO add support for the zero pin of the encoder
class IncrementalEncoder {
	private:
		Gpio& encoderA;
		Gpio& encoderB;
		Timer& tim;

		int counter;
		int last_value;
		bool reverse;
	public:
		//Must be called often enough (less than half a timer cycle has been made since last update)
		IncrementalEncoder& update();
		IncrementalEncoder(Gpio& a, Gpio& b, Timer& t, int prescaler=0, bool reverse=false);
		int getCounter();
		operator int();
		int operator=(int);
};
#endif /* _INCREMENTAL_ENCODER_H */
