#ifndef _AX12_H
#define _AX12_H

#include <Uart.h>

class Ax12 {
	private:
		Uart uart;
		int id;
		void prelude();
		void writeReg(char reg, char val);
		void writeReg(char reg, unsigned short val);
		void readReg(char reg);
		void putChecksum();
		void put(char v);
		char checksum;
		void getMsg();
	public:
		Ax12(Uart u, int i);
		Ax12& setLed(bool);
		Ax12& ping();
};

#endif /* _AX12_H */
