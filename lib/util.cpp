extern "C" {
	char *memset(char *s, int c, int n) {
		while(n--)
			s[n]=c;
		return s;
	}

	char *memcpy(char *d, char *s, int n){
		while(n--)
			d[n]=s[n];
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
	void* pvPortMalloc(unsigned int);
	void vPortFree(void*);
};

void *operator new (unsigned int n)  throw() {
	return pvPortMalloc(n);
}

void operator delete(void* ptr) throw() {
	vPortFree(ptr);
}
