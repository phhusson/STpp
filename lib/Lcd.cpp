#include <Lcd.h>
#include <Debug.h>
extern "C" void vTaskDelay(int);

LcdLine::LcdLine() {
	lcd = NULL;
	line = -1;
	locked = false;
	position = 0;
}

void LcdLine::set(Lcd& lcd, int line) {
	this->lcd = &lcd;
	this->line = line;
}

LcdLine& LcdLine::put(char c) {
	if(!locked) {
		lcd->lock.lock();
		lcd->setDDRAM(line * 40);
		position = 0;
		locked = true;
	}
	lcd->put(c);
	position++;
	return *this;
}

LcdLine& LcdLine::endl() {
	if(locked) {
		for(int i=0; i<(16-position); ++i)
			lcd->put(' ');
		lcd->lock.unlock();
		locked = false;
	}
	return *this;
}

Lcd::Lcd(Gpio RS, Gpio E, Gpio DB7, Gpio DB6, Gpio DB5, Gpio DB4) :
	RS(RS), E(E), DB7(DB7), DB6(DB6), DB5(DB5), DB4(DB4),
	is_4bit(true) {
	init();
	clearDisplay();
	displaySet(true, true, true);
	setDirection(true, false);

	first.set(*this, 0);
	second.set(*this, 1);
}

static void setupGpio(Gpio& a) {
	a
		.setSpeed(Gpio::SPEED_100MHz)
		.setPushPull()
		.setDirection(Gpio::OUTPUT);
}

void Lcd::wait(bool quick) {
	//TODO: have proper functions
	if(quick)
		vTaskDelay(1);
	else
		vTaskDelay(4);
}

void Lcd::init() {
	if(!is_4bit) while(true);
	setupGpio(RS);
	setupGpio(E);
	setupGpio(DB7);
	setupGpio(DB6);
	setupGpio(DB5);
	setupGpio(DB4);

	//Set to 8bit mode
	for(int i=0; i<3; ++i) {
		write4(0, 0b011);
		wait(false);
	}

	//Set to 4 bit mode
	write4(0, 0b0010);
	wait(false);

	functionSet(false, true, false);
}

void Lcd::write4(int rs, int val) {
	RS = !!rs;
	DB4 = !!( (val>>0) &1);
	DB5 = !!( (val>>1) &1);
	DB6 = !!( (val>>2) &1);
	DB7 = !!( (val>>3) &1);
	wait(true);
	E = true;
	wait(true);
	E = false;
	wait(true);
}

void Lcd::write(int rs, int val) {
	if(!is_4bit) while(1);
	write4(rs, val>>4);
	write4(rs, val&0xf);
}

Lcd& Lcd::functionSet(bool is_8bits, bool is_2lines, bool font) {
	int cmd = 0b00100000;
	cmd |= is_8bits ? (1<<4) : 0;
	cmd |= is_2lines ? (1<<3) : 0;
	cmd |= font ? (1<<2) : 0;
	write(0, cmd);
	return *this;
}

Lcd& Lcd::clearDisplay() {
	AutoLock l(lock);
	write(0, 0b00000001);
	return *this;
}

Lcd& Lcd::returnHome() {
	AutoLock l(lock);
	write(0, 0b00000010);
	return *this;
}

Lcd& Lcd::displaySet(bool on, bool cursor, bool blink) {
	AutoLock l(lock);
	int cmd = 0b00001000;
	cmd |= blink ? 1 : 0;
	cmd |= cursor ? 2 : 0;
	cmd |= on ? 4 : 0;
	write(0, cmd);
	return *this;
}

Lcd& Lcd::setDirection(bool increment, bool display) {
	AutoLock l(lock);
	int cmd = 0b00000100;
	cmd |= display ? 1 : 0;
	cmd |= increment ? 2 : 0;
	write(0, cmd);
	return *this;
}

Lcd& Lcd::move(bool cursor, bool left) {
	AutoLock l(lock);
	int cmd = 0b00010000;
	cmd |= left ? (1 << 2) : 0;
	cmd |= cursor ? (2 << 2) : 0;
	write(0, cmd);
	return *this;
}

Lcd& Lcd::put(const char *str) {
	while(*str)
		write(1, *str++);
	return *this;
}

Lcd& Lcd::put(char c) {
	write(1, c);
	return *this;
}

LcdLine& Lcd::operator()(int c) {
	if(c == 0)
		return first;
	return second;
}

Lcd& Lcd::setDDRAM(int addr) {
	addr&=0x7f;
	write(0, addr|0x80);
	wait(false);
	return *this;
}

Lcd& Lcd::setCGRAM(int addr) {
	addr&=0x3f;
	write(0, addr|0x40);
	return *this;
}
