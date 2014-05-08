#include <IncrementalEncoder.h>

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

int IncrementalEncoder::operator=(int v) {
	if(reverse)
		v = -v;
	counter = v;
	return -counter;
}

//Compute absolute coder's position, assuming the previous position is less than half a timer's cycle away
IncrementalEncoder& IncrementalEncoder::update() {
	int v = tim.getCounter();

	//Assume we're going forward
	int d = v - last_value;
	if(d <= 0)
		d+=65536;


	if(d>=32768) {
		//We're too far, we were wrong
		//Meaning we're actally going backward
		d = d - 65536;
	}
	counter += d;

	last_value = v;
	return *this;
}

int IncrementalEncoder::getCounter(){
	update();
	if(reverse)
		return -counter;
	return counter;
}

IncrementalEncoder::operator int() {
	return getCounter();
}

