#ifndef _LOG_H
#define _LOG_H

class Log {
	class endl_type {
	};
	private:
		char log[32][128];
		int line;
		int position;
	public:
		static endl_type endl;
		Log& operator<<(char);
		Log& operator<<(const char*);
		Log& operator<<(int);
		Log& operator<<(endl_type);
};

extern Log log;
#endif /* _LOG_H */
