#ifndef _TIMER_H
#define _TIMER_H
#include <stm32f4xx.h>

class Timer {
	private:
		volatile TIM_TypeDef* base;
		int number;
	public:
		Timer(volatile void* b, n) : base(b), number(n) {};
		//All timers
		void setPrescaler(unsigned short);
		void setAutoReload(unsigned short);
		void setAutoReloadBuffered(bool);

		void setCounter(unsigned short);
		unsigned short getCounter();

		void setOneShot(bool);
		void setUpdateDisable(bool);

		void enable();
		void disable();

		enum Direction {
			OUTPUT,
			INPUT1,
			INPUT2,
			INPUT3
		};
		void setChannelDirection(Direction);
		void setChannelCompareBuffered(bool);

		enum ChannelMode {
			MatchHigh,
			MatchLow,
			CompareHigh,
			CompareLow
		};
		void setChannelMode(ChannelMode);
		void setChannelOutput(bool);
		void setChannelComparator(unsigned short);
};

#endif /* _TIMER_H */
