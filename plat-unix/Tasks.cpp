#include "Tasks.h"
#include <pthread.h>
#include <stdlib.h>

static void *callFn(void *t) {
	Task *f = (Task*) t;
	f->f();
	return NULL;
}

Task::Task(Function f, const char *name, int priority, int stackSize): f(f) {
	pthread_create((pthread_t*)&_id, NULL, callFn, this);
}

void Task::suspend() {
	//Not supported...
	exit(1);
}

void Task::resume() {
	//Not supported...
	exit(1);
}

void Task::yield() {
	pthread_yield();
}
