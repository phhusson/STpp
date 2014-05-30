#include <IRRemote.h>
#include <Exti.h>
#include <Log.h>
#include <Log.h>

static const int remoteCodes[][2] = {
	//Digits
	{ 0x00ff6897, 0},
	{ 0x00ff30cf, 1},
	{ 0x00ff18e7, 2},
	{ 0x00ff7a85, 3},
	{ 0x00ff10ef, 4},
	{ 0x00ff38c7, 5},
	{ 0x00ff5aa5, 6},
	{ 0x00ff42bd, 7},
	{ 0x00ff4ab5, 8},
	{ 0x00ff52ad, 9},

	//Extra digits
	{ 0x00ff9867, 20}, //100+
	{ 0x00ffb04f, 21}, //200+

	//Other
	{ 0x00ffa25d, 100}, // Channel -
	{ 0x00ff629d, 101}, // Channel
	{ 0x00ffe21d, 102}, // Channel +

	{ 0x00ff22dd, 110}, // Previous
	{ 0x00ff02fd, 111}, // Next
	{ 0x00ffc23d, 112}, // Pause

	{ 0x00ffe01f, 120}, // Volume -
	{ 0x00ffa857, 121}, // Volume +
	{ 0x00ff906f, 122}, // Equalizer

};

IRRemote::IRRemote(Timer& t, Gpio& g) :
	t(t), g(g) {
	
	g
		.setDirection(Gpio::INPUT)
		.setSpeed(Gpio::SPEED_100MHz)
		.setResistor(Gpio::PULL_UP)
		.setFunction(Gpio::GPIO);

	t
		.setPrescaler(10*168/2)
		.setAutoReload(0xffff)
		.setOneShot(true);

	tmpWord = 0;
	word = 0;
	Exti(g)
		.enableFalling()
		.enableRising()
		.setTopCB([&](int) {
			if(g) {
				t.disable();
				t.setCounter(0);
				t.enable();
				return;
			}

			int v = t.getCounter();

			if(v < 0x25) {
				tmpWord = 0;
			} else if(v < 0x40) {
				tmpWord <<= 1;
			} else if(v < 0x80) {
				tmpWord = 0;
			} else if(v < 0xb0) {
				tmpWord <<= 1;
				tmpWord |= 1;
			} else {
				if( (tmpWord&0x00ff0000) == 0x00ff0000) {
					if(dataReleased.tryTake()) {
						word = tmpWord;
						dataAvailable.give();
					}
					log << "[IR] Got " << (int)tmpWord << endl;
				}
				tmpWord = 0;
			}
		})
		.enableIRQ();
	
	dataAvailable.tryTake();
	dataReleased.tryTake();
}

int IRRemote::nextRaw() {
	//Start registering events only at the start of this function
	dataReleased.give();
	dataAvailable.take();
	int v = word;

	return v;
}

int IRRemote::next() {
	int v = nextRaw();
	for(unsigned i=0; i<(sizeof(remoteCodes)/sizeof(remoteCodes[0])); ++i) {
		if(remoteCodes[i][0] == v)
			return remoteCodes[i][1];
	}
	return -1;
}
