#include <Timer.h>

Timer::Timer(volatile TIM_TypeDef* b, int n) :
	base(b), number(n) {
	//TIM1: APB2 0
	//TIM8: APB2 1
	//TIM9: APB2 16
	//TIM10: APB2 17
	//TIM11: APB2 18
	//
	//TIM2: APB1 0
	//...
	//TIM7: APB1 5
	//TIM12: APB1 6
	//...
	//TIM14: APB1 8
	switch(n) {
		case 1:
			SET_BIT(RCC->APB2ENR, 0);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			SET_BIT(RCC->APB1ENR, n-2);
			break;
		case 8:
			SET_BIT(RCC->APB2ENR, 1);
			break;
		case 9:
		case 10:
		case 11:
			SET_BIT(RCC->APB2ENR, n+7);
			break;
		case 12:
		case 13:
		case 14:
			SET_BIT(RCC->APB1ENR, n-6);
			break;
	};
}

int Timer::getNumber() {
	return number;
}

unsigned short Timer::getAutoReload() {
	return base->ARR;
}

void Timer::setAutoReload(unsigned short v) {
	base->ARR = v;
}

void Timer::setPrescaler(unsigned short v) {
	base->PSC = v;
}

void Timer::setAutoReloadBuffered(bool v) {
	if(v) {
		base->CR1 |= TIM_CR1_ARPE;
	} else {
		base->CR1 &= ~TIM_CR1_ARPE;
	}
}

void Timer::setCounter(unsigned short v) {
	base->CNT = v;
}

unsigned short Timer::getCounter() {
	return base->CNT;
}

void Timer::enable() {
	base->CR1 |= TIM_CR1_CEN;
}

void Timer::disable() {
	base->CR1 &= ~TIM_CR1_CEN;
}

void Timer::setChannelDirection(int chan, Direction d) {
	chan--;
	int v=(int)d;
	volatile uint16_t *CCMR=&(base->CCMR1);
	if(chan>=2) {
		CCMR+=2;
		chan-=2;
	}
	int offset = 8*chan;
	*CCMR = (*CCMR & ~(3 << offset)) |
		v << offset;
}

void Timer::setChannelMode(int chan, ChannelMode m) {
	chan--;
	int v;
	switch(m) {
		case MatchHigh:
			v=1;
			break;
		case MatchLow:
			v=2;
			break;
		case CompareHigh:
			v=6;
			break;
		case CompareLow:
			v=7;
			break;
	}

	volatile uint16_t *CCMR=&(base->CCMR1);
	if(chan>=2) {
		CCMR+=2;
		chan-=2;
	}
	int offset = 8*chan + 4;
	*CCMR = (*CCMR & ~(7<<offset)) |
		v << offset;
}

void Timer::setChannelOutput(int chan, bool o) {
	chan--;
	if(o) {
		base->CCER |= 1 << (4*chan);
	} else {
		base->CCER &= ~(1 << (4*chan));
	}
}

void Timer::setChannelComparator(int chan, unsigned short v) {
	chan--;
	volatile unsigned int *b=&(base->CCR1);
	b+=chan;
	*b=v;
}