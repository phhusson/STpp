#ifndef _AX12_H
#define _AX12_H

#include <Uart.h>

class Ax12 {
	private:
		Uart uart;
		int id;
		Gpio *g;
		void prelude();
		void writeReg(char reg, char val);
		void writeReg(char reg, unsigned short val);
		void readReg(char reg);
		void putChecksum();
		void put(char v);
		char checksum;
		void getMsg();
		bool deferred;
		void flush();
	public:
		//One wire
		Ax12(Gpio g, Uart u, int i);
		//Three wires
		Ax12(Uart u, int i, Gpio *sw, Gpio& rx, Gpio& tx);
		Ax12& setLed(bool);
		Ax12& ping();

		Ax12& defer();
		Ax12& action(bool broadcast = false);

		Ax12& setID(char);
		char getID();

		Ax12& setCWLimit(unsigned short);
		Ax12& getCWLimit(unsigned short);
		Ax12& setCCWLimit(unsigned short);
		Ax12& getCCWLimit(unsigned short);

		Ax12& setEndless();
		Ax12& clearEndless();

		Ax12& setMaxTorque(unsigned short, bool persist=false);

		Ax12& enable();
		Ax12& disable();

		Ax12& goTo(unsigned short);
		Ax12& setSpeed(unsigned short);
};

#endif /* _AX12_H */
