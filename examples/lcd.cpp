#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Lcd.h>
extern "C" void vTaskDelay(int);

void lcd_write4(int rs, int rw, int word) {
	//rw not wired
	Lcd_RS = rs;
	Lcd_DB4 = !!((word>>0)&1);
	Lcd_DB5 = !!((word>>1)&1);
	Lcd_DB6 = !!((word>>2)&1);
	Lcd_DB7 = !!((word>>3)&1);
	vTaskDelay(1);
	Lcd_E = true;
	vTaskDelay(1);
	Lcd_E = false;
	vTaskDelay(1);
}

int main() {
	Lcd lcd(Lcd_RS, Lcd_E, Lcd_DB7, Lcd_DB6, Lcd_DB5, Lcd_DB4);
	lcd
		.displaySet(true, true, true)
		.setDirection(false, true)
		.puts("aaabbb");


		//Display on, with cursor blinking
#if 0
	Lcd_RS
		.setSpeed(Gpio::SPEED_100MHz)
		.setPushPull()
		.setDirection(Gpio::OUTPUT);
	
	Lcd_E
		.setSpeed(Gpio::SPEED_100MHz)
		.setPushPull()
		.setDirection(Gpio::OUTPUT);

	Lcd_DB4
		.setSpeed(Gpio::SPEED_100MHz)
		.setPushPull()
		.setDirection(Gpio::OUTPUT);

	Lcd_DB5
		.setSpeed(Gpio::SPEED_100MHz)
		.setPushPull()
		.setDirection(Gpio::OUTPUT);

	Lcd_DB6
		.setSpeed(Gpio::SPEED_100MHz)
		.setPushPull()
		.setDirection(Gpio::OUTPUT);

	Lcd_DB7
		.setSpeed(Gpio::SPEED_100MHz)
		.setPushPull()
		.setDirection(Gpio::OUTPUT);


	Lcd_RS = Lcd_E = Lcd_DB4 = Lcd_DB5 = Lcd_DB6 = Lcd_DB7 = false;

		int i;
		//Set to 8 bit mode
		for(i=0;i<3;++i) {
			lcd_write4(0, 0, 0b0011);
			vTaskDelay(4);
		}

		//Set to 4 bit mode
		lcd_write4(0, 0, 0b0010);
		//lcd_write4(0, 0, 0b1100);
		vTaskDelay(4);

		//Now in 4 bit mode
		//Nline = 2, font = 5x11
		lcd_write4(0, 0, 0b0010);
		lcd_write4(0, 0, 0b0000);

		//Display on
		//lcd_write4(0, 0, 0b0011);
		//lcd_write4(0, 0, 0b1000);
		//
		//lcd_write4(0, 0, 0b0011);
		//lcd_write4(0, 0, 0b0001);
		//
		//Entry Mode Set
		//lcd_write4(0, 0, 0b0011);
		//lcd_write4(0, 0, 0bC1DS);

		vTaskDelay(2);
		lcd_write4(0, 0, 0);
		lcd_write4(0, 0, 0b0111);

		//Clear display
		lcd_write4(0, 0, 0b0000);
		lcd_write4(0, 0, 0b0001);

		vTaskDelay(7);
		lcd_write4(1, 0, 'a'>>4);
		lcd_write4(1, 0, 'a'&0xf);
		lcd_write4(1, 0, 'a'>>4);
		lcd_write4(1, 0, 'a'&0xf);
		vTaskDelay(3);
#endif
	while(1)
		vTaskDelay(1000);
}
