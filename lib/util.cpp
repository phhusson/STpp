extern "C" {
#include "FreeRTOS.h"
#include "portable.h"
	char *memset(char *s, int c, int n) {
		volatile char *p = s;
		while(n--)
			p[n]=c;
		return s;
	}

	char *memcpy(char *d, char *s, int n){
		char *p=d;
		while(n--)
			p[n]=s[n];
		return d;
	}

	char *strncpy(char *d, char *s, int n) {
		int i;
		for(i=0;i<n;++i) {
			d[i]=s[i];
			if(d[i]==0)
				break;
		}
		return d;
	}	
};

void *operator new (unsigned int n)  throw() {
	return pvPortMalloc(n);
}

void operator delete(void* ptr) throw() {
	vPortFree(ptr);
}
