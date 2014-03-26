#include "Tasks.h"

static void callFn(void *t) {
	Task *f = (Task*) t;
	f->f();
}

Task::Task(Function f, const char *name, int priority, int stackSize): f(f) {
	int ret = xTaskCreate(callFn, (const signed char*)name, stackSize, this, priority, &_id);
	if(ret != pdPASS)
		while(1);
}

void Task::suspend() {
	vTaskSuspend(_id);
}

void Task::resume() {
	vTaskResume(_id);
}

void Task::yield() {
	taskYIELD();
}
