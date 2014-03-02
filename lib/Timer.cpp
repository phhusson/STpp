#include <Timer.h>
#include <Log.h>

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

Timer& Timer::setAutoReload(unsigned short v) {
	base->ARR = v;
	return *this;
}

Timer& Timer::setPrescaler(unsigned short v) {
	base->PSC = v;
	return *this;
}

Timer& Timer::setAutoReloadBuffered(bool v) {
	if(v) {
		base->CR1 |= TIM_CR1_ARPE;
	} else {
		base->CR1 &= ~TIM_CR1_ARPE;
	}
	return *this;
}

Timer& Timer::setCounter(unsigned short v) {
	base->CNT = v;
	return *this;
}

unsigned short Timer::getCounter() {
	return base->CNT;
}

Timer& Timer::enable() {
	base->CR1 |= TIM_CR1_CEN;
	return *this;
}

bool Timer::enabled() {
	return !!(base->CR1&TIM_CR1_CEN);
}

Timer& Timer::disable() {
	base->CR1 &= ~TIM_CR1_CEN;
	return *this;
}

Timer& Timer::setChannelDirection(int chan, Direction d) {
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
	return *this;
}

Timer& Timer::setChannelMode(int chan, ChannelMode m) {
	chan--;
	int v = 1;
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
	return *this;
}

Timer& Timer::setChannelOutput(int chan, bool o) {
	chan--;
	if(o) {
		base->CCER |= 1 << (4*chan);
	} else {
		base->CCER &= ~(1 << (4*chan));
	}
	return *this;
}

Timer& Timer::setChannelComparator(int chan, unsigned short v) {
	chan--;
	volatile unsigned int *b=(volatile unsigned int*)&(base->CCR1);
	b+=chan;
	*b=v;
	return *this;
}

Timer& Timer::wait() {
	while(*this);
	return *this;
}

Timer& Timer::setOneShot(bool v) {
	if(v) {
		base->CR1 |= 1<<3;
	} else {
		base->CR1 &= ~(1<<3);
	}
	return *this;
}

Timer::operator bool() {
	return !!(base->CR1 & 1);
}

Timer& Timer::update() {
	base->EGR |= 1;
	return *this;
}

Timer& Timer::setCaptureCompare1Sel(int v){
	base->CCMR1 &= ~TIM_CCMR1_CC1S;
	base->CCMR1 |= v%4;
	return *this;
}

Timer& Timer::setCaptureCompare2Sel(int v){
	base->CCMR1 &= ~TIM_CCMR1_CC2S;
	base->CCMR1 |= (v%4) << 8;
	return *this;
}

Timer& Timer::setCaptureCompare1Polarity(bool in, int v){
	base->CCER &= ~TIM_CCER_CC1P;
	base->CCER |= (v&0x1) << 1;
	// CC1NP is only allowed to be changed in input mode	
	if(in){
		base->CCER &= ~TIM_CCER_CC1NP;
		base->CCER |= (v&0x2) << 3;
	} 
	return *this;
}

Timer& Timer::setCaptureCompare2Polarity(bool in, int v){
	base->CCER &= ~TIM_CCER_CC2P;
	base->CCER |= (v&0x1) << 5;
	// CC2NP is only allowed to be changed in input mode	
	if(in){
		base->CCER &= ~TIM_CCER_CC2NP;
		base->CCER |= (v&0x2) << 7;
	} 
	return *this;
}

Timer& Timer::setInputCapture1Filter(int v){
	base->CCMR1 &= ~TIM_CCMR1_IC1F;
	base->CCMR1 |= (v%16) << 4;
	return *this;
}

Timer& Timer::setInputCapture2Filter(int v){
	base->CCMR1 &= ~TIM_CCMR1_IC2F;
	base->CCMR1 |= (v%16) << 12;
	return *this;
}

Timer& Timer::setSlaveModeSelection(int v){
	base->SMCR &= ~TIM_SMCR_SMS;
	base->SMCR |= (v%8);
	return *this;
}


//Interrupt handling
Timer& Timer::setUIE(bool s) {
	if(s)
		base->DIER |= 1;
	else
		base->DIER &= ~1;
	return *this;
}

Timer& Timer::setURS(bool onlyOverflow) {
	if(onlyOverflow)
		base->CR1 |= 4;
	else
		base->CR1 &= ~4;
	return *this;
}

Timer& Timer::clearInterrupt() {
	return *this;
}

static Timer::Callback topCBs[15];
void Timer::callTopCb(int nr) {
	topCBs[nr](nr);
}

Timer& Timer::setTopCB(Callback cb) {
	topCBs[number] = cb;
	return *this;
}

extern "C" {
	void TIM1_BRK_TIM9_IRQHandler() {
		TIM9->SR &= ~1;
		Timer::callTopCb(9);
	}

	void TIM1_UP_TIM10_IRQHandler() {
		TIM10->SR &= ~1;
		Timer::callTopCb(10);
	}

	void TIM1_TRG_COM_TIM11_IRQHandler() {
		TIM11->SR &= ~1;
		Timer::callTopCb(11);
	}

	void TIM2_IRQHandler() {
		TIM2->SR &= ~1;
		Timer::callTopCb(2);
	}

	void TIM3_IRQHandler() {
		TIM3->SR &= ~1;
		Timer::callTopCb(3);
	}

	void TIM4_IRQHandler() {
		TIM4->SR &= ~1;
		Timer::callTopCb(4);
	}

	void TIM5_IRQHandler() {
		TIM5->SR &= ~1;
		Timer::callTopCb(5);
	}

	void TIM6_DAC_IRQHandler() {
		TIM6->SR &= ~1;
		Timer::callTopCb(6);
	}

	void TIM7_IRQHandler() {
		TIM7->SR &= ~1;
		Timer::callTopCb(7);
	}

	void TIM8_BRK_TIM12_IRQHandler() {
		TIM12->SR &= ~1;
		Timer::callTopCb(12);
	}

	void TIM8_UP_TIM13_IRQHandler() {
		TIM13->SR &= ~1;
		Timer::callTopCb(13);
	}

	void TIM8_TRG_COM_TIM14_IRQHandler() {
		TIM14->SR &= ~1;
		Timer::callTopCb(14);
	}
};
