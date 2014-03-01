#include <IncrementalEncoder.h>

IncrementalEncoder::IncrementalEncoder(Gpio a, Gpio b, Timer t, int lim, int c):
	encoderA(a), encoderB(b), tim(t), count(c) {
	int n_tim = tim.getNumber();
	if(n_tim<=2){
		encoderA.setAlternate(Gpio::TIM1_2);
		encoderB.setAlternate(Gpio::TIM1_2);
	}
	else if(n_tim<=5){
		encoderA.setAlternate(Gpio::TIM3_5);
		encoderB.setAlternate(Gpio::TIM3_5);
	}

	tim
		.setPrescaler(0)
		.setAutoReload(lim)
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
	}

int IncrementalEncoder::getCounter(){
	return tim.getCounter();
}

IncrementalEncoder::operator int() {
	return tim.getCounter();
}

