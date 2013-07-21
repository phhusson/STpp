#include <Ax12.h>
#include <Log.h>

extern "C" void vTaskDelay(int);
Ax12::Ax12(Uart u, int i) :
	uart(u), id(i) {

	uart
		.enable()
		//~1Mbps
		.setMantissa(2)
		.setFraction(0xb)
		.setHalfDuplex(true)
		.enableReceive()
		.enableTransmitter();

	writeReg(0x06, (unsigned short)0);
	writeReg(0x08, (unsigned short)0x1ff);

	writeReg(0x20, (unsigned short)0x3ff);
	writeReg(0x18, (char)1);
	writeReg(0x1E, (unsigned short)0x1ff);
}

void Ax12::prelude() {
	uart
		.put(0xff)
		.put(0xff)
		.put(id);
	checksum=id;
}

void Ax12::put(char v) {
	uart.put(v);
	checksum+=v;
}

void Ax12::putChecksum() {
	uart.put(~checksum);
}

void Ax12::readReg(char reg) {
	prelude();
	//N args
	put(4);
	//Write
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
	put(0x03);
	put(reg);
	put(val);
	putChecksum();
	getMsg();
	getMsg();
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
}

void Ax12::getMsg() {
	//12 should be enough... right ?
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
	log << "end (found " << (int)checksum << Log::endl;
}
