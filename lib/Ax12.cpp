#include <Ax12.h>
#include <Log.h>

static char buf[16] __attribute((section("dma")));
static char *bufPos = buf;
Ax12::Ax12(Gpio g, Uart u, int i) :
	uart(u), id(i), g(0) {
	g
		.setPushPull()
		.setAlternate(Gpio::USART1_3)
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz)
		.setResistor(Gpio::PULL_UP);

	uart
		.enable()
		//~1Mbps
		//.setMantissa(2)
		//.setFraction(0xb)
		//a,b,c
		.setMantissa(0x16)
		.setFraction(0x6)
		.setHalfDuplex(true)
		.enableReceive()
		.enableTransmitter();
	deferred = false;
	bufPos = buf;
}

Ax12::Ax12(Uart u, int i, Gpio *g) : uart(u), id(i), g(g) {
	g
		->setPushPull()
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz)
		.setResistor(Gpio::PULL_UP);

	uart
		.enable()
		//~1Mbps
		.setMantissa(2)
		.setFraction(0xb)
		//a,b,c
		//.setMantissa(0x16)
		//.setFraction(0x6)
		//.setHalfDuplex(true)
		.enableReceive()
		.enableTransmitter();
	deferred = false;
	bufPos = buf;
	*g = false;
}

Ax12& Ax12::defer() {
	deferred = true;
	return *this;
}

Ax12& Ax12::setCWLimit(unsigned short v) {
	writeReg(0x06, v);
	return *this;
}

Ax12& Ax12::setCCWLimit(unsigned short v) {
	writeReg(0x08, v);
	return *this;
}

Ax12& Ax12::setEndless() {
	setCWLimit(0);
	setCCWLimit(0);
	return *this;
}

Ax12& Ax12::clearEndless() {
	setCWLimit(0);
	setCCWLimit(0x1ff);
	return *this;
}

Ax12& Ax12::enable() {
	writeReg(0x18, (char)1);
	return *this;
}

Ax12& Ax12::disable() {
	writeReg(0x18, (char)0);
	return *this;
}

Ax12& Ax12::goTo(unsigned short v) {
	writeReg(0x1E, v);
	return *this;
}

Ax12& Ax12::setSpeed(unsigned short v) {
	v&=0x7ff;
	writeReg(0x20, v);
	return *this;
}

void Ax12::flush() {
	uart.put(buf, bufPos-buf);
}

void Ax12::prelude() {
	*bufPos++ = 0xff;
	*bufPos++ = 0xff;
	*bufPos++ = id;
	checksum=id;
}

void Ax12::put(char v) {
	*bufPos++ = v;
	if(bufPos >= buf + sizeof(buf))
		while(1);
	checksum+=v;
}

void Ax12::putChecksum() {
	*bufPos++ = ~checksum;
	flush();
	bufPos = buf;
}

void Ax12::readReg(char reg) {
	prelude();
	//N args
	put(3);
	//Read
	put(0x02);
	put(reg);
	putChecksum();
	getMsg();
	getMsg();
}

void Ax12::writeReg(char reg, char val) {
	prelude();
	//N args
	put(4);
	//Write
	put(deferred ? 0x04 : 0x03);
	put(reg);
	put(val);
	putChecksum();
	getMsg();
	getMsg();
	deferred = false;
}

Ax12& Ax12::action(bool broadcast) {
	checksum = 0;
	*bufPos++ = 0xff;
	*bufPos++ = 0xff;
	put(broadcast ? 0xfe : id);
	put(2);
	put(0x05);
	putChecksum();
	getMsg();
	if(!broadcast)
		getMsg();

	return *this;
}

void Ax12::writeReg(char reg, unsigned short val) {
	prelude();
	//N args
	put(5);
	//Write
	put(0x03);
	put(reg);
	put(val&0xff);
	put(val>>8);
	putChecksum();
	getMsg();
	getMsg();
}

Ax12& Ax12::setLed(bool v) {
	writeReg(0x19, v ? (char)1 : (char)0);
	return *this;
}

Ax12& Ax12::ping() {
	prelude();
	put(2);
	put(1);
	putChecksum();
	getMsg();

	return *this;
}

void Ax12::getMsg() {
	//12 should be enough... right ?
	if(id == 0xfe)
		return;
	if(g)
		*g = true;

	char msg[12];
	msg[3]=0xff;
	int i;
	for(i=0 ; i< (msg[3]+4) ; ++i) {
		msg[3]&=~0x80;
		msg[i] = uart.waitForNext();
		log << (int)msg[i] << ":";
	}
	int j;
	char checksum = 0;
	for(j=2;j<(i-1);++j)
		checksum+=msg[j];
	checksum=~checksum;
	log << "end (found " << (int)checksum << endl;
	if(g)
		*g = false;
}

Ax12& Ax12::setMaxTorque(unsigned short int value, bool persist) {
	if(persist)
		writeReg(0x0e, value);
	writeReg(0x22, value);
	return *this;
}

Ax12& Ax12::setID(char id) {
	writeReg(3, id);
	return *this;
}
