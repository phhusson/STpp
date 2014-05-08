#include <IncrementalEncoder.h>

/**
 * @brief Creates an IncrementalEncoder object
 *
 * @params a           Gpio of the A pin of the Incremental Encoder
 * @params b           Gpio of the B pin of the Incremental Encoder
 * @params t           Timer on which the encoder's gpios are plugged
 * @params prescaler   Timer prescaler
 * @params reverse     Set to 1 of the pins A and B are switched (as the timer has no way of knowing which one is A or B)
 *
 */
IncrementalEncoder::IncrementalEncoder(Gpio& a, Gpio& b, Timer& t, int prescaler, bool reverse):
	encoderA(a), encoderB(b), tim(t), reverse(reverse) {

	tim.setAlternate(encoderA);
	tim.setAlternate(encoderB);

	tim
		.setPrescaler(prescaler)
		.setAutoReload(0xffff)
		.setCaptureCompare1Sel(1)
		.setCaptureCompare2Sel(1)
		.setCaptureCompare1Polarity(1,0)
		.setCaptureCompare2Polarity(1,0)
		.setInputCapture1Filter(0)
		.setInputCapture2Filter(0)
		.setSlaveModeSelection(3)
		.enable();

	encoderA
		.setSpeed(Gpio::SPEED_100MHz)
		.setDirection(Gpio::INPUT)
		.setResistor(Gpio::PULL_UP);
	encoderB

		.setSpeed(Gpio::SPEED_100MHz)
		.setDirection(Gpio::INPUT)
		.setResistor(Gpio::PULL_UP);

	counter = 0;
	last_value = 0;
}

/**
 * @brief Sets the counter value to the argument value
 *
 */
int IncrementalEncoder::operator=(int v) {
	counter = reverse ? -v : v;
	return v;
}

/**
 * @brief Computes the absolute coder's position
 *
 * @details assuming the previous position is less than half a timer's cycle away
 *
 */
IncrementalEncoder& IncrementalEncoder::update() {
	int v = tim.getCounter();

	// Assume we're going forward
	int d = v - last_value;
	if(d <= 0)
		d+=65536;


	if(d >= 32768) {
		// We're too far, we were wrong
		// Meaning we're actally going backward
		d = d - 65536;
	}
	counter += d;

	last_value = v;
	return *this;
}

/**
 * @brief Gives out the current absolute value of the counter
 *
 */
int IncrementalEncoder::getCounter(){
	update();
	if(reverse)
		return -counter;
	return counter;
}

/**
 * @brief Gets the value from the getCounter function call 
 *
 */
IncrementalEncoder::operator int() {
	return getCounter();
}

