#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Lcd.h>

int main() {
	Lcd lcd(Lcd_RS, Lcd_E, Lcd_DB7, Lcd_DB6, Lcd_DB5, Lcd_DB4);

	lcd(0) << "aaaabb" << endl;
	lcd(1) << "aaaabb" << endl;

	while(1)
		time.msleep(1000);
}
