#include "Shell.h"

Shell::Shell(IStream& in, OStream& out): in(in), out(out),
	cbs_name{}, n_cbs(0) {

	cbs[0]=[](Stack &s) {
		int a = s.pop().toInt();
		int b = s.pop().toInt();
		s.push(a+b);
	};
	cbs_name[0][0] = "+";
	cbs_name[0][1] = 0;

	cbs[1]=[&out](Stack &s) {
		Object& o = s.pop();
		out << "# " << o.toInt() << endl;
	};
	cbs_name[1][0] = ":";
	cbs_name[1][1] = 0;

	n_cbs = 2;
}

bool Shell::isValue(const char *str) {
	if(*str >= '0' && *str <= '9')
		return true;
	if(*str == '"')
		return true;
	return false;
}

void Shell::pushInt(const char *str) {
	if(str[0] != '0' || str[1] != 'x')
		while(1);
	int v = 0;
	for(int i = 2; str[i]; ++i) {
		char c = str[i];
		if(c >= '0' && c <= '9')
			c = c - '0';
		else if(c >= 'a' && c <= 'f')
			c = c - 'a' + 10;
		else if(c >= 'A' && c <= 'F')
			c = c - 'A' + 10;
		else
			while(1);
		c <<= 4;
		v |= c;
	}
	s.push(v);
}

void Shell::pushStr(const char *str) {
	while(1);
}

void Shell::pushValue(const char *str) {
	if(*str >= '0' && *str <= '9')
		pushInt(str);
	else if(*str == '"')
		pushStr(str);
}

static bool fnc_eq(const char *str, const char **name) {
	int j = 0, i = 0;
	for(i=0;str[i];++i) {
		if(!j) {
			if(str[i] != name[0][i] && str[i] != '.')
				return false;
			if(str[i] == '.') {
				j = i;
				if(!name[1])
					return false;
			}
		} else {
			if(str[i] != name[1][i-j] && str[i] != '.')
				return false;
		}
	}
	if(!j) {
		if(str[i] != name[0][i])
			return false;
	} else
		if(str[i] != name[1][i-j])
			return false;
	return true;
}

void Shell::call(const char *str) {
	for(int i=0; i<n_cbs; ++i) {
		if(fnc_eq(str, cbs_name[i])) {
			cbs[i](s);
			return;
		}
	}
	out << "Function not found !" << endl;
}

void Shell::exec(const char* prompt) {
	char cmd[256];
	while(true) {
		out << prompt;
		in >> cmd;
		if(isValue(cmd)) {
			pushValue(cmd);
		} else {
			call(cmd);
		}
	}
}
