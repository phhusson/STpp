#ifndef LCD_H
#define LCD_H
#include <Gpio.h>

class Lcd {
	private:
		void write(int r, int val);
		void write4(int r, int val);
		void init();
		void wait(bool quick);
		Gpio RS, E, DB7, DB6, DB5, DB4;
		bool is_4bit;
	public:
		Lcd(Gpio RS, Gpio E, Gpio DB7, Gpio DB6, Gpio DB5, Gpio DB4);
		// 0 0 0 0 0 0 0 1
		Lcd& clearDisplay();
		// 0 0 0 0 0 0 1 *
		Lcd& returnHome();
		// 0 0 0 0 0 1 I D
		Lcd& setDirection(bool increment, bool display);
		// 0 0 0 0 1 O C B
		Lcd& displaySet(bool on, bool cursor, bool blink);
		// 0 0 0 1 x x * *
		Lcd& move(bool cursor, bool left);
	private:
		// 0 0 1 8 N F * *
		Lcd& functionSet(bool is_8bits, bool is_2lines, bool font);
	public:
		//0 1 A5 A4 A3 A2 A1 A0
		Lcd& setCGRAM(int addr);
		//1 A6 A5 A4 A3 A2 A1 A0
		Lcd& setDDRAM(int addr);
		
		Lcd& puts(const char* str);

};
#endif
