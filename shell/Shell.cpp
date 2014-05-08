#include "Shell.h"

static NullIStream nullis;
static NullOStream nullos;

Shell::Shell() : cbs_name{}, n_cbs(0),
	in(&nullis), out(&nullos),
	history{}, history_pos(0) {

	current_object = 0;
	got_name = false;

	add([](Stack &s) {
			int a = s.pop().toInt();
			int b = s.pop().toInt();
			s.push(a+b);
		}, "+");

	add([this](Stack &s) {
			Object& o = s.pop();
			*(this->out) << "# " << o.toInt() << endl;
		}, ".");

	add([this](Stack &s) {
			s.push(1);
		}, "on");

	add([this](Stack &s) {
			s.push(0);
		}, "off");
}

void Shell::add(Callback cb, const char *name1, const char *name2) {
	cbs_name[n_cbs][0] = name1;
	cbs_name[n_cbs][1] = name2;
	cbs[n_cbs] = cb;
	n_cbs++;
	if(n_cbs == N_FUNCTIONS)
		while(1);
}

bool Shell::isValue(const char *str) {
	if((*str >= '0' && *str <= '9') || *str =='-')
		return true;
	if(*str == '"')
		return true;
	return false;
}

static int parseHex(const char *str) {
	int v = 0;
	for(int i = 0; str[i]; ++i) {
		char c = str[i];
		if(c >= '0' && c <= '9')
			c = c - '0';
		else if(c >= 'a' && c <= 'f')
			c = c - 'a' + 10;
		else if(c >= 'A' && c <= 'F')
			c = c - 'A' + 10;
		else
			while(1);
		v <<= 4;
		v |= c;
	}
	return v;
}

static int parseDec(const char *str) {
	int v = 0;
	for(int i = 0; str[i]; ++i) {
		char c = str[i];
		if(c >= '0' && c <= '9')
			c = c - '0';
		else
			while(1);
		v *= 10;
		v += c;
	}
	return v;
}

static int parseOct(const char *str) {
	int v = 0;
	for(int i = 0; str[i]; ++i) {
		char c = str[i];
		if(c >= '0' && c <= '7')
			c = c - '0';
		else
			while(1);
		v <<= 3;
		v |= c;
	}
	return v;
}

void Shell::pushInt(const char *str) {
	bool opposite = false;
	if(str[0]=='-') {
		opposite=true;
		str++;
	}
	int v = 0;
	if(str[0] == '0') {
		//Could be octal or hexa
		if(str[1] == 'x')
			v = parseHex(str+2);
		else
			v = parseOct(str+1);

	} else {
		v = parseDec(str);
	}
	if(opposite)
		v = -v;
	s.push(v);
}

void Shell::pushStr(const char *str) {
	while(1);
	(void)str;
}

void Shell::pushValue(const char *str) {
	if((*str >= '0' && *str <= '9') || *str == '-')
		pushInt(str);
	else if(*str == '"')
		pushStr(str);
}

static int str_cmp_split(const char *str, const char *to, char key=':') {
	for(int i=0;str[i] || to[i]; ++i) {
		if(str[i] == key && to[i] == 0)
			return i+1;
		if(str[i] != to[i])
			return 0;
	}
	return -1;
}

static bool fnc_eq(const char *str, const char **name) {
	int ret;
	ret = str_cmp_split(str, name[0]);
	if(!ret)
		return false;
	if(ret == -1 && name[1] == 0)
		return true;
	if(!name[1])
		return false;
	return !!str_cmp_split(str+ret, name[1], 0);
}

void Shell::call(const char *str) {
	for(int i=0; i<n_cbs; ++i) {
		if(fnc_eq(str, cbs_name[i])) {
			cbs[i](s);
			return;
		}
	}
	*out << "Function not found !" << endl;
	*out << "\t'" << str << "'" << endl;
}

static int str_part_cmp_split(const char *str, const char *to, char key=':') {
	for(int i=0;str[i] || to[i]; ++i) {
		if(str[i] == key && to[i] == 0)
			return i+1;
		if(str[i] == 0)
			return -1;
		if(str[i] != to[i])
			return 0;
	}
	return -1;
}

static bool fnc_part_match(const char *str, const char **name) {
	int ret;
	ret = str_part_cmp_split(str, name[0]);
	if(!ret)
		return false;
	if(ret == -1)
		return true;
	if(!name[1])
		return false;
	return !!str_part_cmp_split(str+ret, name[1], 0);
}

void Shell::completion(char *partial, int& i) {
	int start = i;
	for(start=i; start>=0; --start) if(partial[start]== ' ') break;
	start++;

	char longest[LINE_SIZE];
	longest[0] = 0;
	int count = 0;
	for(int j=0; j<n_cbs; ++j) {
		if(fnc_part_match(partial+start, cbs_name[j])) {
			*out << cbs_name[j][0];
			if(cbs_name[j][1])
				*out << ':' << cbs_name[j][1];
			*out << endl;
			if(count == 0) {
				int k,l;
				l = 0;
				for(k=0; cbs_name[j][0][k]; ++k)
					longest[k] = cbs_name[j][0][k];
				if(cbs_name[j][1]) {
					longest[k++] = ':';
					for(l=0; cbs_name[j][1][l]; ++l)
						longest[k+l] = cbs_name[j][1][l];
				}
				longest[k+l] = 0;
			} else {
				int k,l;
				l=0;
				for(k=0; cbs_name[j][0][k]; ++k) {
					if(cbs_name[j][0][k] != longest[k]) {
						longest[k] = 0;
						break;
					}
				}

				if(cbs_name[j][1] && longest[k] == ':' ) {
					++k;
					for(l=0; cbs_name[j][1][l]; l++) {
						if(cbs_name[j][1][l] != longest[k+l]) {
							longest[k+l] = 0;
							break;
						}
					}
				}
				longest[k+l] = 0;
			}
			count++;
		}
	}

	i=start;
	for(int j=0; j<LINE_SIZE; ++j) {
		if( (j+start) > LINE_SIZE)
			break;
		partial[j+start] = longest[j];
		if(longest[j] == 0)
			break;
		++i;
	}
	--i;
	if(count == 0) {
		*out << "No match found" << endl;
	}
}

void Shell::exec(bool echo, const char* prompt) {
	char cmd[LINE_SIZE];
	while(true) {
		*out << prompt;
		for(int i=0; i<int(sizeof(cmd)-2); ++i) {
			*in >> cmd[i];
			// \r\n
			if(cmd[i] == '\r' || cmd[i] == '\n') {
				cmd[i] = 0;
				break;
			} else if(cmd[i] == 0) {
				break;
			}

			//Backspace
			if(cmd[i] == '\b') {
				if(i!=0) {
					*out << "\b \b";
					cmd[i--] = 0;
				}
				cmd[i--] = 0;
			//ctrl-c
			} else if(cmd[i] == 3) {
				*out << endl;
				*out << prompt;
				i = 0;
			//Tab
			} else if(cmd[i] == '\t') {
				*out << endl;
				cmd[i] = 0;
				i--;
				completion(cmd, i);
				*out << prompt;
				*out << cmd;
			//ctrl-d
			} else if(cmd[i] == 0x4) {
				parseWord("reset");
				--i;
			//Escape
			} else if(cmd[i] == 0x1b) {
				*in >> cmd[i];
				//Unhandled
				if(cmd[i] != '[')
					while(1);
				*in >> cmd[i];
				if(cmd[i] == 'A') {
					history_pos--;
					if(history_pos < 0)
						history_pos = 3;
					for(i=0;history[history_pos][i];++i) {
						cmd[i] = history[history_pos][i];
					}
					cmd[i]=0;
					*out << "\r\x1b[K";
					*out << prompt;
					*out << cmd;
				}
				--i;
			} else 	if(echo) {
				*out << cmd[i];
			}
		}
		*out << endl;
		int i = 0;
		for(i=0; cmd[i]; ++i)
			history[history_pos][i] = cmd[i];
		history[history_pos][i] = 0;
		history_pos++;
		history_pos %= 4;

		char *current = cmd;
		char *pos = cmd;
		for(; *pos; ++pos) {
			if(*pos == ' ') {
				*pos = 0;
				parseWord(current);
				current = pos+1;
			}
		}
		parseWord(current);
	}
}

void Shell::parseWord(const char* word) {
	if(isValue(word)) {
		pushValue(word);
	} else {
		call(word);
	}
}

Shell& Shell::operator<<(const char* name) {
	if(!got_name) {
		got_name = true;
		current_object = name;
	} else {
		while(1);
	}
	return *this;
}

void Shell::setStream(IStream* _in, OStream* _out) {
	this->in = _in;
	this->out = _out;
}
