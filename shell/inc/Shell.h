#ifndef _SHELL_H
#define _SHELL_H

#include "IStream.h"
#include "OStream.h"
#include <tr1/functional>

class Object {
	public:
		enum {
			NONE,
			INT,
			STRING,
			PTR
		} type;
		void *store;
		inline int toInt() { if(type != INT) while(1); return (int) store; }
		inline const char* toString() { if(type != STRING) while(1); return (const char*) store; }
		inline void* toPtr() { if(type != PTR) while(1); return (void*) store; }

		inline void operator()(int v) { type = INT; store = (void*)v; }
		inline void operator()(const char *s) { type = STRING; store = (void*)s; }
		inline void operator()(void *p) { type = PTR; store = (void*)p; }
		inline void operator()() { type = NONE; }
		inline Object() { type = NONE; }
};

class Stack {
	private:
		Object s[16];
		int n;
	public:
		inline Stack() { n = 0; }
		inline Object& pop() { if(n<=0) while(1); return s[--n];}
		template<class T> void push(T v) { s[n++](v); }
};

class Shell {
	private:
		typedef std::tr1::function<void(Stack&)> Callback;
		Stack s;

#define N_FUNCTIONS 128
		Callback cbs[N_FUNCTIONS];
		const char *cbs_name[N_FUNCTIONS][2];
		int n_cbs; 

		bool isValue(const char *str);
		void pushInt(const char *str);
		void pushStr(const char *str);
		void pushValue(const char *str);
		void call(const char *cmd);
		void completion(char *partial, int &i);

	public:
		//To be used by overloaders
		const char *current_object;
		bool got_name;
		void add(Callback cb, const char *name1, const char *name2=0);
		IStream* in;
		OStream* out;
	public:
		Shell();
		void setStream(IStream* in, OStream* out);
		void exec(bool echo = true, const char* prompt="> ");
		Shell& operator<<(const char*);

		void parseWord(const char *);
	private:
#define LINE_SIZE 80
		char history[4][256];
		int history_pos;
};

//Helpers
#define addSetter(SHELL, OBJ, FUNC) { \
	SHELL.add([&OBJ,&SHELL](Stack& s) { \
		Object& o = s.pop(); \
		if(o.type != Object::INT) while(1); \
		int v = o.toInt(); \
		OBJ.FUNC(v); \
	}, SHELL.current_object, #FUNC); \
}

#endif /* _SHELL_H */
