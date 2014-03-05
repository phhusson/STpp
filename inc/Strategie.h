#ifndef _STRATEGIE_H
#define _STRATEGIE_H
#include <Asserv.h>
#include <Ax12.h>

class Strategie {
	private:
		Ax12& mamoutor_servo;
		Asserv& asserv;

	public:
		Strategie(Ax12&, Asserv&);
		void run();

		void mamoutor();
		void resetMamoutor();

		void reset();
};

#endif /* _STRATEGIE_H */
