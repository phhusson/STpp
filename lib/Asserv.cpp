#include <Irq.h>
#include <Log.h>
#include "Asserv.h"

Asserv::Asserv(IncrementalEncoder& left, IncrementalEncoder& right,
	Timer& tim) : left(left), right(right), tim(tim) {
	tim
		.setPrescaler(42)
		.setAutoReload(1000)
		.setOneShot(false)
		.setUIE(true)
		.setURS(true);

	Irq(tim.irqNr())
		.setPriority(15)
		.enable();
	tim
		.setTopCB([&left, &right](int timer_id) {
			left.update();
			right.update();
		})
		.enable();
}
