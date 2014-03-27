#include <Board.h>
#include <Tasks.h>
#include <iostream>

class Wheel {
	private:
		Timer& encoder;
		Timer& HBridge;
		int HBridge_nb;
	public:
		Wheel(Timer& encoder, Timer& HBridge, int nb);
};

Wheel LeftW(Tim3, Tim2, 3);
Wheel::Wheel(Timer& encoder, Timer& HBridge, int nb) :
	encoder(encoder), HBridge(HBridge), HBridge_nb(nb) {
	new Task([this]() {
		while(true) {
			time.msleep(1);
			this->encoder.setCounter(this->encoder.getCounter()+this->HBridge.getChannelComparator(this->HBridge_nb));
		}
	}, "Wheel Simulator");
}
