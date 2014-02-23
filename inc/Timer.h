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
		Timer& setPrescaler(unsigned short);
		Timer& setAutoReload(unsigned short);
		unsigned short getAutoReload();
		Timer& setAutoReloadBuffered(bool);

		Timer& setCounter(unsigned short);
		Timer& wait();
		unsigned short getCounter();

		Timer& setOneShot(bool);
		Timer& setUpdateDisable(bool);

		Timer& enable();
		Timer& disable();
		bool enabled();

		int getNumber();

		//Not all timers
		enum Direction {
			OUTPUT,
			INPUT1,
			INPUT2,
			INPUT3
		};
		Timer& setChannelDirection(int,Direction);
		Timer& setChannelCompareBuffered(int,bool);

		enum ChannelMode {
			MatchHigh,
			MatchLow,
			CompareHigh,
			CompareLow
		};
		Timer& setChannelMode(int,ChannelMode);
		Timer& setChannelOutput(int,bool);
		Timer& setChannelComparator(int,unsigned short);
		Timer& update();
		Timer& setCaptureCompare1Sel(int);
		Timer& setCaptureCompare2Sel(int);
		Timer& setCaptureCompare1Polarity(bool, int);
		Timer& setCaptureCompare2Polarity(bool, int);
		Timer& setInputCapture1Filter(int);
		Timer& setInputCapture2Filter(int);	
		Timer& setSlaveModeSelection(int);

		operator bool();
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
