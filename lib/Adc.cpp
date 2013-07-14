#include <Adc.h>
#include <stm32f4xx.h>

Adc::Adc() {
	//Enable ADC
	ADC1->CR2 |= ADC_CR2_ADON;

	//Disable EOCIE
	ADC1->CR1 &= ~ADC_CR1_EOCIE;

	//Set TSVREFE bit
	ADC->CCR|= ADC_CCR_TSVREFE;

	//Clock =APB1/8
	ADC->CCR |= 2<<16;
}

void Adc::setSamples(int chan, Samples s) {
	int v=0;
	switch(s) {
		case N_480:
			v++;
		case N_144:
			v++;
		case N_112:
			v++;
		case N_84:
			v++;
		case N_56:
			v++;
		case N_28:
			v++;
		case N_15:
			v++;
		case N_3:
			break;
	};
	if(chan>9) {
		chan-=9;
		ADC1->SMPR1= (ADC1->SMPR1 & ~(7<<(3*chan))) | v << (3*chan);
	} else {
		ADC1->SMPR2= (ADC1->SMPR2 & ~(7<<(3*chan))) | v << (3*chan);
}
	}

unsigned int Adc::oneShot(int chan) {
	//Enable ADC clock

	//See procedure p267

	//Only one conversion
	ADC1->SQR1=0;
	ADC1->SQR3=chan;

	//12bits
	ADC1->CR1 &= 3 << 24;

	//Trigger
	ADC1->CR2 |= ADC_CR2_SWSTART;
	
	//Wait for end of conversion
	while( ! (ADC1->SR & ADC_SR_EOC));
	//Read data
	unsigned int val = ADC1->DR&0xfff;
	ADC1->CR2 &= ~ADC_CR2_SWSTART;

	return val;
}

float Adc::getTemperature() {
	setSamples(16, N_480);
	unsigned int ret = oneShot(16);
	
	//Temp = (vsense - v25)/avg_slope + 25
	return ret;
}
