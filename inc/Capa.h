#ifndef _CAPA_H
#define _CAPA_H

class Capa {
	private:
		int n;
		Gpio pins[8];
		int values[8];
		void prepare(int);
		int offset[8];
	public:
		Capa();
		Capa& add(Gpio);

		//Pre load capacitors
		Capa& prepare();
		//Actually do the measurement
		Capa& measure();
		Capa& update();

		//Use as a function to update
		inline void operator()() {
			update();
		}
		inline int operator[](int i) {
			return values[i];
		}
};

#endif /* _CAPA_H */
