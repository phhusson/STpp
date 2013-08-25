#ifndef _MEAN_H
#define _MEAN_H

class Mean {
	private:
		int n;
		int val;
		bool inited;
	public:
		Mean(int);
		Mean& operator+=(int);
		operator int();
};

#endif /* _MEAN_H */

