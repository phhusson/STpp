#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Lcd.h>
extern "C" void vTaskDelay(int);

int main() {
	Lcd lcd(Lcd_RS, Lcd_E, Lcd_DB7, Lcd_DB6, Lcd_DB5, Lcd_DB4);
	lcd
		.displaySet(true, true, true)
		.setDirection(false, true);

	lcd(0) << "aaaa" << endl;

	while(1)
		vTaskDelay(1000);
}
