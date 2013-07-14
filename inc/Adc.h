#ifndef _ADC_H
#define _ADC_H

class Adc {
	public:
		Adc();
		unsigned int oneShot(int chan);
		enum Samples {
			N_3,
			N_15,
			N_28,
			N_56,
			N_84,
			N_112,
			N_144,
			N_480,
		};
		void setSamples(int chan, Samples s);
		float getTemperature();
};
#endif /* _ADC_H */
