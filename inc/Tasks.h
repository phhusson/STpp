#ifndef _TASKS_H
#define _TASKS_H

extern "C" {
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
};
#include <tr1/functional>

class Task {
	public:
		typedef std::tr1::function<void(void)> Function;
		Function f;
		xTaskHandle _id;
		Task(Function& f, const char *name, int priority = tskIDLE_PRIORITY+2, int stackSize = 256);
		void suspend();
		void resume();
		static void yield();
};

#endif /* _TASKS_H */
