#ifndef _TIMER_H
#define _TIMER_H
#include <stm32f4xx.h>

class Timer {
	private:
		volatile TIM_TypeDef* base;
		int number;
	public:
		Timer(volatile TIM_TypeDef* b, int n);
		//All timers
		void setPrescaler(unsigned short);
		void setAutoReload(unsigned short);
		unsigned short getAutoReload();
		void setAutoReloadBuffered(bool);

		void setCounter(unsigned short);
		unsigned short getCounter();

		void setOneShot(bool);
		void setUpdateDisable(bool);

		void enable();
		void disable();

		int getNumber();

		//Not all timers
		enum Direction {
			OUTPUT,
			INPUT1,
			INPUT2,
			INPUT3
		};
		void setChannelDirection(int,Direction);
		void setChannelCompareBuffered(int,bool);

		enum ChannelMode {
			MatchHigh,
			MatchLow,
			CompareHigh,
			CompareLow
		};
		void setChannelMode(int,ChannelMode);
		void setChannelOutput(int,bool);
		void setChannelComparator(int,unsigned short);
};

extern Timer Tim1;
extern Timer Tim2;
extern Timer Tim3;
extern Timer Tim4;
extern Timer Tim5;
extern Timer Tim6;
extern Timer Tim7;
extern Timer Tim8;
extern Timer Tim9;
extern Timer Tim10;
extern Timer Tim11;
extern Timer Tim12;
extern Timer Tim13;
extern Timer Tim14;
#endif /* _TIMER_H */
